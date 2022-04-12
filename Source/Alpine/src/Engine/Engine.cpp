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
		
		//D3D11_RASTERIZER_DESC wireFrameDesc = {};
		//wireFrameDesc.FillMode = D3D11_FILL_SOLID;
		//wireFrameDesc.CullMode = D3D11_CULL_BACK;
		//wireFrameDesc.AntialiasedLineEnable = true;
		//myDevice->CreateRasterizerState(&wireFrameDesc, &myWireFrame);
		//myContext->RSSetState(myWireFrame);




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
		DX11::GetDeviceContext()->OMSetRenderTargets(1, DX11::GetAdressOfRenderTargetView(), DX11::GetDepthStencilView());

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
		myCubeMap->Bind(9);
		ID3D11UnorderedAccessView* myUAV;
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.Flags = 0;
		uavDesc.Buffer.NumElements = 6;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		uavDesc.Texture2DArray.ArraySize = 6;
		uavDesc.Texture2DArray.FirstArraySlice = 0;
		uavDesc.Texture2DArray.MipSlice = 0;
		auto hr = DX11::GetDevice()->CreateUnorderedAccessView(myTexture->GetResource().Get(), &uavDesc, &myUAV);
		assert(hr == S_OK && "Failed to create UAV");
		DX11::GetDeviceContext()->CSSetShader(myComputeShader.GetShader(), 0, 0);
		DX11::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &myUAV, 0);
		DX11::GetDeviceContext()->Dispatch(32, 32, 1);

		DX11::GetDeviceContext()->CSSetShader(0, 0, 0);
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
		myImguiLayer.Begin();
		DX11::ClearView();
		auto ratio = Application::GetWindow()->GetAspectRatio();
		myCamera.SetAspectRatio(ratio);
		myCameraBufferObject.position = Vector4(myCamera.GetPosition().x, myCamera.GetPosition().y, myCamera.GetPosition().z, 1);
		myCameraBufferObject.toCameraSpace = myCamera.GetViewMatrix();
		myCameraBufferObject.toProjectionSpace = myCamera.GetProjectionMatrix();
		myCameraBuffer.SetData(&myCameraBufferObject, sizeof(CameraBuffer));
		myCameraBuffer.Bind();


		ImGui::BeginMainMenuBar();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::EndMainMenuBar();
		ImGui::Begin("Light");
		
		static float dir[] = { -1, -1, -1 };
		ImGui::SliderFloat3("Light Direction", dir, -1.f, 1.f);
		static float intensity = 5.f;
		ImGui::SliderFloat("Light Intensity", &intensity, 1.f, 30.f);
		ImGui::End();
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

		myImguiLayer.RenderImGui();
		myImguiLayer.End();
		
	}

	void Engine::CleanD3D()
	{
		myImguiLayer.OnDetach();
		DX11::CleanUpDX11();
	}
}