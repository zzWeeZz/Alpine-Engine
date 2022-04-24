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
	Engine::Engine() : m_ModelBuffer(1), m_CameraBuffer(0), m_LightBuffer(2), m_SpecBuffer(5)
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
		m_IrrComputeShader.Initialize("Shaders/IrradianceMap_cs.cso");
		m_SpecularComputeShader.Initialize("Shaders/SpeclularMap_cs.cso");
		m_SpbrdfShader.Initialize("Shaders/spdrdfGenerator_cs.cso");

		DX11::Context()->VSSetShader(m_VertexShader.GetShader(), 0, 0);
		DX11::Context()->PSSetShader(m_PixelShader.GetShader(), 0, 0);
	
		FramebufferSpecification spec = {};
		spec.width = Application::GetWindowSize().x;
		spec.height = Application::GetWindowSize().y;
		
		m_FrameBuffer = FrameBuffer::Create(spec);
		m_CubeMap = TextureCube::Create("Textures/Storforsen4");
		ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
		m_SpecBuffer.Create();
		m_SpecularMap = TextureCube::Create(1024, 1024, DXGI_FORMAT_R32G32B32A32_FLOAT);
		// specular map
		DX11::Context()->CSSetShaderResources(0, 1, m_CubeMap->GetShaderResourceView().GetAddressOf());
		DX11::Context()->CSSetShader(m_SpecularComputeShader.GetShader(), 0, 0);
		const float deltaRoughness = 1.0f / std::max((float)(m_CubeMap->GetLevels() - 1), 1.0f);
		for (UINT level = 1, size = 512; level < m_CubeMap->GetLevels(); ++level, size /= 2)
		{
			const UINT numGroups = std::max<UINT>(1, size / 32);
			m_SpecularMap->CreateUAV(level);
			const SpectularMapFilerSettingsBuffer al = { level * deltaRoughness };
			DX11::Context()->UpdateSubresource(m_SpecBuffer.GetBuffer(), 0, nullptr, &al, 0, 0);
			m_SpecBuffer.Bind(true, 0);
			DX11::Context()->CSSetUnorderedAccessViews(0, 1, m_SpecularMap->GetUnorderedAccessView().GetAddressOf(), nullptr);
			DX11::Context()->Dispatch(numGroups, numGroups, 1);
		}
		DX11::Context()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
		m_SpecularMap->Bind(10);
		// irradians
		m_IrMap = TextureCube::Create(32, 32, DXGI_FORMAT_R32G32B32A32_FLOAT, 1);
		m_IrMap->CreateUAV();
		m_CubeMap->Bind(9, true);
		DX11::Context()->CSSetShaderResources(0, 1, m_CubeMap->GetShaderResourceView().GetAddressOf());
		DX11::Context()->CSSetUnorderedAccessViews(0, 1, m_IrMap->GetUnorderedAccessView().GetAddressOf(), 0);
		DX11::Context()->CSSetShader(m_IrrComputeShader.GetShader(), 0, 0);
		DX11::Context()->Dispatch(1, 1, 1);
		DX11::Context()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
		m_IrMap->Bind(11);

		// spdrbf
		m_Texture = Texture::Create(256, 256, DXGI_FORMAT_R16G16_FLOAT, 1);
		m_Texture->CreateUAV();
		DX11::Context()->CSSetUnorderedAccessViews(0, 1, m_Texture->GetUnorderedAccessView().GetAddressOf(), 0);
		DX11::Context()->CSSetShader(m_SpbrdfShader.GetShader(), 0, 0);
		DX11::Context()->Dispatch(256/32, 256 / 32, 1);
		DX11::Context()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
		m_Texture->Bind(6);
		
		D3D11_RASTERIZER_DESC cmDesc = {};
		cmDesc.FillMode = D3D11_FILL_SOLID;
		cmDesc.CullMode = D3D11_CULL_BACK;
		cmDesc.FrontCounterClockwise = true;
		cmDesc.DepthClipEnable = false;
		DX11::Device()->CreateRasterizerState(&cmDesc, &m_CCWcullMode);
		cmDesc.FrontCounterClockwise = false;
		DX11::Device()->CreateRasterizerState(&cmDesc, &m_CWcullMode);

		D3D11_RASTERIZER_DESC rastDesc = {};
		cmDesc.FillMode = D3D11_FILL_SOLID;
		cmDesc.CullMode = D3D11_CULL_NONE;
		DX11::Device()->CreateRasterizerState(&rastDesc, &m_NoCull);

		D3D11_BLEND_DESC blendDesc = {};
		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
		renderTargetBlendDesc.BlendEnable = true;
		renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = renderTargetBlendDesc;

		DX11::Context()->IASetInputLayout(m_VertexShader.GetInputLayout());
		auto topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		DX11::Context()->IAGetPrimitiveTopology(&topology);
		DX11::Context()->RSSetState(m_CCWcullMode);
		
		m_CameraBuffer.Create();
		m_LightBuffer.Create();
		m_ModelBuffer.Create();
		InitObjects();
	}

	void Engine::InitObjects()
	{
		m_Camera.Init({ 0,50, 100 });
	
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

		m_Ground.LoadModel("Cube", m_GroundMaterial);
		m_Ground.SetScale({ 200, 10, 200 });
		m_ImguiLayer.OnAttach();
		
	}

	void Engine::Update(float aDeltaTime)
	{
		m_Camera.Update(aDeltaTime);
		static float rotation = 0;
		rotation += aDeltaTime * 100.f;
		m_RockMan.SetRotation({ 0, -rotation / 2, 0 });
		m_MetalMan.SetRotation({ 0, -rotation / 2, 0 });
	}


	void Engine::RenderFrame()
	{
		m_FrameBuffer->Bind();
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

		m_ModelBuffer.SetData(&m_Ground.GetTransform(), sizeof(Matrix));
		m_ModelBuffer.Bind();
		m_Ground.Draw();
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
		ImGui::Begin("ViewPort", &pOpen, ImGuiWindowFlags_NoCollapse);
		if (m_FrameBuffer->GetSpecification().width != ImGui::GetWindowWidth() || m_FrameBuffer->GetSpecification().height != ImGui::GetWindowHeight())
		{
			m_FrameBuffer->Resize(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
			m_Camera.SetAspectRatio(ImGui::GetWindowWidth() / ImGui::GetWindowHeight());
		}
		ImGui::Image(m_FrameBuffer->GetColorAttachment(), { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });

		ImGui::End();
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