#include "Engine.h"

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "ToolBox/Input/Input.h"
#include <assimp/Importer.hpp>
#include "DX11/DX11.h"
#include "Application.h"

namespace Alpine
{
	Engine::Engine() : myModelBuffer(1), myConstantBuffer(0), myLightBuffer(2)
	{

	}
	void Engine::InitD3D(HWND aHWND, int aScreenWidth, int aScreenHight)
	{
		myScreenHeight = aScreenHight;
		myScreenWidth = aScreenWidth;
		// Inits Direct X
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = aHWND;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			NULL,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			DX11::GetAdressOfSwapChain(),
			DX11::GetAdressOfDevice(),
			NULL,
			DX11::GetAdressOfDeviceContext());



		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Height = Application::GetWindowSize().second;
		viewport.Width = Application::GetWindowSize().first;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		DX11::GetDeviceContext()->RSSetViewports(1, &viewport);


		ID3D11Texture2D* backBufferPtr;
		DX11::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));

		DX11::GetDevice()->CreateRenderTargetView(backBufferPtr, NULL, DX11::GetAdressOfRenderTargetView());

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = Application::GetWindowSize().first;
		depthStencilDesc.Height = Application::GetWindowSize().second;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.MiscFlags = 0;


		DX11::GetDevice()->CreateTexture2D(&depthStencilDesc, NULL, DX11::GetAdressOfDepthStencilBuffer());
		DX11::GetDevice()->CreateDepthStencilView(DX11::GetDepthStencilBuffer(), NULL, DX11::GetAdressOfDepthStencilView());

		myModel.SetModel("Model/helicopter.fbx", L"alfk");
		myModel.GetTransform() *= Matrix::CreateRotationX(-3.141f / 2);
		myModel.GetTransform() *= Matrix::CreateRotationY(-3.141f / 2);
		myModel.GetTransform() *= Matrix::CreateScale({ 1.f, 1.0f, 1.f });
		myModel.GetTransform() *= Matrix::CreateTranslation({ 0,-2.5, -70 });

		myLambo.SetModel("Model/Lamborghini_Aventador.fbx", L"Textures/Lamborginhi Aventador_diffuse.jpeg");
		myLambo.GetTransform() *= Matrix::CreateRotationY(3.14 / 4.f);
		myLambo.GetTransform() *= Matrix::CreateScale(0.1);
		myLambo.GetTransform() *= Matrix::CreateTranslation(Vector3(10, 0, 0));

		myGround.SetModel("Cube", L"Textures/Ground.png");
		myGround.GetTransform() *= Matrix::CreateRotationX(0);
		myGround.GetTransform() *= Matrix::CreateScale({ 200, 10, 200 });
		myGround.GetTransform() *= Matrix::CreateTranslation(Vector3(25, -5, -50));

		myConstantBuffer.Create();
		myLightBuffer.Create();
		myModelBuffer.Create();


		InitPipeline();
	}

	void Engine::InitPipeline()
	{
		myCamera.Init({ 0,0, 20 });
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		myVertexShader.Initialize(L"Shaders/VertexShader.cso", layout, ARRAYSIZE(layout));
		myPixelShader.Initialize(L"Shaders/PixelShader.cso");


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
		renderTargetBlendDesc.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = renderTargetBlendDesc;

		DX11::GetDeviceContext()->IASetInputLayout(myVertexShader.GetInputLayout());
		auto topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		DX11::GetDeviceContext()->IAGetPrimitiveTopology(&topology);
		DX11::GetDeviceContext()->RSSetState(myCCWcullMode);
		DX11::GetDeviceContext()->OMSetRenderTargets(1, DX11::GetAdressOfRenderTargetView(), DX11::GetDepthStencilView());
		myImguiLayer.OnAttach();
	}

	void Engine::Update(float aDeltaTime)
	{
		myCamera.Update(aDeltaTime);
	}


	void Engine::RenderFrame()
	{
		myImguiLayer.Begin();
		float color[4] = { 0.3f,0.f, 3.f, 1.f };
		DX11::GetDeviceContext()->ClearRenderTargetView(DX11::GetRenderTargetView(), color);
		DX11::GetDeviceContext()->ClearDepthStencilView(DX11::GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		auto ratio = Application::GetAspectRatio();
		myCamera.SetAspectRatio(ratio);
		myConstantBufferObject.toCameraSpace = myCamera.GetViewMatrix();
		myConstantBufferObject.toProjectionSpace = myCamera.GetProjectionMatrix();
		myConstantBuffer.SetData(&myConstantBufferObject, sizeof(CameraConstBuffer));
		myConstantBuffer.Bind();

		

		myLightConstantBufferObject.ambientColor = Vector4(myAmbientLight.GetLightColor().x, myAmbientLight.GetLightColor().y, myAmbientLight.GetLightColor().z, 1);
		myLightConstantBufferObject.lights[0] = { Vector4(1, 1, 1, 1), Vector4(-1, -1, -1, 1) };

		myLightBuffer.SetData(&myLightConstantBufferObject, sizeof(LightConstBuffer));
		myLightBuffer.Bind();

		myModelBuffer.SetData(&myModel.GetTransform(), sizeof(Matrix));
		myModelBuffer.Bind();
		myModel.Draw();

		myModelBuffer.SetData(&myLambo.GetTransform(), sizeof(Matrix));
		myModelBuffer.Bind();
		myLambo.Draw();

		myModelBuffer.SetData(&myGround.GetTransform(), sizeof(Matrix));
		myModelBuffer.Bind();
		myGround.Draw();

		myImguiLayer.RenderImGui();
		myImguiLayer.End();
		DX11::GetSwapChain()->Present(0, 0);
	}

	void Engine::CleanD3D()
	{
		myImguiLayer.OnDetach();
		DX11::CleanUpDX11();
	}
}