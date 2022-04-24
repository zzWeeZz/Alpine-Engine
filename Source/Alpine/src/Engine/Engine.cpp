#include "Engine.h"

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <imgui.h>

#include "ToolBox/Input/Input.h"
#include <assimp/Importer.hpp>
#include "DX11/DX11.h"
#include "Application/Application.h"
#include <Effects.h>

namespace Alpine
{
	Engine::Engine() : m_ModelBuffer(1), m_CameraBuffer(0), m_LightBuffer(2)
	{

	}
	void Engine::InitD3D(HWND aHWND, int aScreenWidth, int aScreenHight)
	{
		DX11::Initialize(aScreenWidth, aScreenHight, false);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TBASIS" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		m_VertexShader.Initialize(L"Shaders/pbrShader_vs.cso", layout, ARRAYSIZE(layout));
		m_PixelShader.Initialize(L"Shaders/pbrShader_ps.cso");

		DX11::Context()->VSSetShader(m_VertexShader.GetShader(), 0, 0);
		DX11::Context()->PSSetShader(m_PixelShader.GetShader(), 0, 0);
	
		FramebufferSpecification spec = {};
		spec.width = Application::GetWindowSize().x;
		spec.height = Application::GetWindowSize().y;
		
		m_FrameBuffer = FrameBuffer::Create(spec);
		
		
		DX11::Context()->IASetInputLayout(m_VertexShader.GetInputLayout());
		auto topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		DX11::Context()->IAGetPrimitiveTopology(&topology);
		
		m_CameraBuffer.Create();
		m_LightBuffer.Create();
		m_ModelBuffer.Create();
		InitObjects();
	}

	void Engine::InitObjects()
	{
		m_Camera.Init({ 0,0, 0 });
	
		m_MetalicMaterial = Material::Create("Metalic");
		m_MetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-albedo.png"));
		m_MetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-roughness.png"));
		m_MetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-normal-dx.png"));
		m_MetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-ao.png"));
		m_MetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-metallic.png"));
		m_MetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-height.png"));
		m_MetalMan.LoadModel("Model/M_MED_Gumshoe_Export.fbx", m_MetalicMaterial);
		m_MetalMan.SetRotation({ 0, 0, 0 });
		m_MetalMan.SetPosition({ 0,10.f, 0 });
		m_MetalMan.SetScale({ 0.1f, 0.1f, 0.1f });

		m_GroundMaterial = Material::Create("Ground");
		m_GroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-albedo.png"));
		m_GroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-roughness.png"));
		m_GroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-normal-dx.png"));
		m_GroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-ao.png"));
		m_GroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-metallic.png"));
		m_GroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-height.png"));
		m_RockMan.LoadModel("Model/M_MED_Gumshoe_Export.fbx", m_GroundMaterial);
		m_RockMan.SetScale({ 0.1f, 0.1f, 0.1f });
		m_RockMan.SetPosition({ 30, 10, 0 });

		m_ImguiLayer.OnAttach();
		m_FrameBuffer->Resize(256, 256);
		m_FrameBuffer->Bind();
		m_SkyBox = SkyBox::Create(L"Textures/environment.hdr");

	}

	void Engine::Update(float aDeltaTime)
	{
		m_Camera.Update(aDeltaTime);
		float rotation = 0;
		rotation += aDeltaTime * 100.f;
		m_RockMan.SetRotation({ m_RockMan.GetRotation().x, m_RockMan.GetRotation().y - rotation / 2, m_RockMan.GetRotation().z });
		m_MetalMan.SetRotation({ m_MetalMan.GetRotation().x, m_MetalMan.GetRotation().y - rotation / 2, m_MetalMan.GetRotation().z });
	}


	void Engine::RenderFrame()
	{
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Pixel, 0);
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Clamp, ShaderType::Pixel, 1);
		m_FrameBuffer->Bind();
		m_SkyBox->Bind();
		m_ImguiLayer.Begin();
		DX11::ClearView();
		m_FrameBuffer->ClearView({ 0.3f, 0, 3, 1 });
		m_FrameBuffer->ClearDepthStencil();
	
		m_CameraBufferObject.position = Vector4(m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z, 1);
		m_CameraBufferObject.toCameraSpace = m_Camera.GetViewMatrix();
		m_CameraBufferObject.toProjectionSpace = m_Camera.GetProjectionMatrix();
		m_CameraBuffer.SetData(&m_CameraBufferObject, sizeof(CameraBuffer));
		m_CameraBuffer.Bind();
		static float dir[] = { -1, -1, -1 };
		
		static float intensity = 5.f;

		m_LightBufferObject.ambientColor = Vector4(m_AmbientLight.GetLightColor().x, m_AmbientLight.GetLightColor().y, m_AmbientLight.GetLightColor().z, 1);
		m_LightBufferObject.lights[0] = { Vector4(1, 1, 1, intensity), Vector4(dir[0], dir[1], dir[2], 0)};

		m_LightBuffer.SetData(&m_LightBufferObject, sizeof(LightBuffer));
		m_LightBuffer.Bind();

		m_ModelBuffer.SetData(&m_MetalMan.GetTransform(), sizeof(Matrix));
		m_ModelBuffer.Bind();
		m_MetalMan.Draw();

		m_ModelBuffer.SetData(&m_RockMan.GetTransform(), sizeof(Matrix));
		m_ModelBuffer.Bind();
		m_RockMan.Draw();
		

		m_FrameBuffer->UnBind();
		DX11::Bind();
		m_ImguiLayer.RenderImGui();
		static bool pOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &pOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false))
					pOpen = false;
				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("ViewPort", &pOpen, ImGuiWindowFlags_NoCollapse);
		if (m_FrameBuffer->GetSpecification().width != ImGui::GetWindowWidth() || m_FrameBuffer->GetSpecification().height != ImGui::GetWindowHeight())
		{
			m_FrameBuffer->Resize(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
			m_Camera.SetAspectRatio(ImGui::GetWindowWidth() / ImGui::GetWindowHeight());
		}
		ImGui::Image(m_FrameBuffer->GetColorAttachment(), { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
		ImGui::End();
		ImGui::PopStyleVar();
		m_SkyBox->Check();
		ImGui::Begin("Inspector");
		if(ImGui::CollapsingHeader("Transfrom"))
		{
			static float heliPos[3] = { m_MetalMan.GetPosition().x, m_MetalMan.GetPosition().y, m_MetalMan.GetPosition().z };
			ImGui::DragFloat3("Position", heliPos);
			m_MetalMan.SetPosition({ heliPos[0], heliPos[1], heliPos[2] });
			
			static float rot[3];
			ImGui::DragFloat3("Rotation", rot);
			
			m_MetalMan.SetRotation({ fmodf(rot[0], 360), fmodf(rot[1], 360), fmodf(rot[2], 360) });
		}
		ImGui::End();
		ImGui::ShowDemoWindow();
		ImGui::Begin("Objects");
		ImGui::End();
		
		ImGui::End();
		m_ImguiLayer.End();

	}

	void Engine::CleanD3D()
	{
		m_ImguiLayer.OnDetach();
		DX11::CleanUpDX11();
	}
}