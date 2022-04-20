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
	Engine::Engine() : myModelBuffer(1), myCameraBuffer(0), myLightBuffer(2)
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
		myVertexShader.Initialize(L"Shaders/VertexShader.cso", layout, ARRAYSIZE(layout));
		myPixelShader.Initialize(L"Shaders/PixelShader.cso");
		myComputeShader.Initialize("Shaders/ComputeShader.cso");

		DX11::GetDeviceContext()->VSSetShader(myVertexShader.GetShader(), 0, 0);
		DX11::GetDeviceContext()->PSSetShader(myPixelShader.GetShader(), 0, 0);
		
	
		FramebufferSpecification spec = {};
		spec.width = Application::GetWindowSize().x;
		spec.height = Application::GetWindowSize().y;
		
		myFrameBuffer = FrameBuffer::Create(spec);



		D3D11_RASTERIZER_DESC cmDesc = {};
		cmDesc.FillMode = D3D11_FILL_SOLID;
		cmDesc.CullMode = D3D11_CULL_BACK;
		cmDesc.FrontCounterClockwise = true;
		cmDesc.DepthClipEnable = false;
		DX11::GetDevice()->CreateRasterizerState(&cmDesc, &myCCWcullMode);
		cmDesc.FrontCounterClockwise = false;
		DX11::GetDevice()->CreateRasterizerState(&cmDesc, &myCWcullMode);

		D3D11_RASTERIZER_DESC rastDesc = {};
		cmDesc.FillMode = D3D11_FILL_SOLID;
		cmDesc.CullMode = D3D11_CULL_NONE;
		DX11::GetDevice()->CreateRasterizerState(&rastDesc, &myNoCull);

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

		DX11::GetDeviceContext()->IASetInputLayout(myVertexShader.GetInputLayout());
		auto topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		DX11::GetDeviceContext()->IAGetPrimitiveTopology(&topology);
		DX11::GetDeviceContext()->RSSetState(myCCWcullMode);
		
		myCameraBuffer.Create();
		myLightBuffer.Create();
		myModelBuffer.Create();
		InitObjects();
	}

	void Engine::InitObjects()
	{
		myCamera.Init({ 0,50, 100 });
		myTexture = Texture::Create("Textures/BRDF LUT.png");
		myCubeMap = TextureCube::Create("Textures/Storforsen4");
		myMetalicMaterial = Material::Create("Metalic");
		myMetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-albedo.png"));
		myMetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-roughness.png"));
		myMetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-normal-dx.png"));
		myMetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-ao.png"));
		myMetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-metallic.png"));
		myMetalicMaterial->AddTexture(Texture::Create("Textures/mesh-covered-metal1-height.png"));
		myHeli.LoadModel("Model/helicopter.fbx", myMetalicMaterial);
		myHeli.SetRotation({ -90, -90, 0 });
		myHeli.SetPosition({ 0,0.f, 0 });
		myHeli.SetScale({ 0.5f, 0.5f, 0.5f });

		myGroundMaterial = Material::Create("Ground");
		myGroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-albedo.png"));
		myGroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-roughness.png"));
		myGroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-normal-dx.png"));
		myGroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-ao.png"));
		myGroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-metallic.png"));
		myGroundMaterial->AddTexture(Texture::Create("Textures/wet-stones-with-sand1-height.png"));
		mySphere.LoadModel("Model/M_MED_Gumshoe_Export.fbx", myMetalicMaterial);
		mySphere.SetScale({ 0.1f, 0.1f, 0.1f });
		mySphere.SetPosition({ 0, 10, 30 });

		myGround.LoadModel("Cube", myGroundMaterial);
		myGround.SetScale({ 200, 10, 200 });
		myImguiLayer.OnAttach();
		myTexture->Bind(6);
		myCubeMap->Bind(10);
		myCubeMap->Bind(11);
		
	}

	void Engine::Update(float aDeltaTime)
	{
		myCamera.Update(aDeltaTime);
		static float rotation = 0;
		rotation += aDeltaTime * 100.f;
		mySphere.SetRotation({ 0, -rotation / 2, 0 });
	}


	void Engine::RenderFrame()
	{
		myFrameBuffer->Bind();
		myImguiLayer.Begin();
		DX11::ClearView();
		auto ratio = Application::GetWindow()->GetAspectRatio();
		myCamera.SetAspectRatio(ratio);
		myCameraBufferObject.position = Vector4(myCamera.GetPosition().x, myCamera.GetPosition().y, myCamera.GetPosition().z, 1);
		myCameraBufferObject.toCameraSpace = myCamera.GetViewMatrix();
		myCameraBufferObject.toProjectionSpace = myCamera.GetProjectionMatrix();
		myCameraBuffer.SetData(&myCameraBufferObject, sizeof(CameraBuffer));
		myCameraBuffer.Bind();
		static float dir[] = { -1, -1, -1 };
		
		static float intensity = 5.f;

		myLightBufferObject.ambientColor = Vector4(myAmbientLight.GetLightColor().x, myAmbientLight.GetLightColor().y, myAmbientLight.GetLightColor().z, 1);
		myLightBufferObject.lights[0] = { Vector4(1, 1, 1, intensity), Vector4(dir[0], dir[1], dir[2], 0)};

		myLightBuffer.SetData(&myLightBufferObject, sizeof(LightBuffer));
		myLightBuffer.Bind();

		myModelBuffer.SetData(&myHeli.GetTransform(), sizeof(Matrix));
		myModelBuffer.Bind();
		myHeli.Draw();

		myModelBuffer.SetData(&mySphere.GetTransform(), sizeof(Matrix));
		myModelBuffer.Bind();
		mySphere.Draw();

		myModelBuffer.SetData(&myGround.GetTransform(), sizeof(Matrix));
		myModelBuffer.Bind();
		myGround.Draw();
		myFrameBuffer->UnBind();
		DX11::Bind();
		myImguiLayer.RenderImGui();
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
		ImGui::Begin("ViewPort");
		ImGui::Image(myFrameBuffer->GetColorAttachment(), { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });

		ImGui::End();
		ImGui::Begin("Light");

		ImGui::SliderFloat3("Light Direction", dir, -1.f, 1.f);
		ImGui::SliderFloat("Light Intensity", &intensity, 1.f, 30.f);
		ImGui::End();
		ImGui::End();
		myImguiLayer.End();

	}

	void Engine::CleanD3D()
	{
		myImguiLayer.OnDetach();
		DX11::CleanUpDX11();
	}
}