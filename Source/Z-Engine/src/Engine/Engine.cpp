#include "Engine.h"

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <assimp/Importer.hpp>

namespace Engine
{
	void Engine::InitD3D(HWND aHWND, int aScreenWidth, int aScreenHight)
	{
		myScreenHeight = aScreenHight;
		myScreenWidth = aScreenWidth;
		// Inits Direct X
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = aHWND;
		swapChainDesc.SampleDesc.Count = 4;
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
			&mySwapchain,
			&myDevice,
			NULL,
			&myContext);



		// ---------- 

	


		// Set Render Target
		ID3D11Texture2D* backBufferPtr;
		mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));

		myDevice->CreateRenderTargetView(backBufferPtr, NULL, &myRenderTargetView);
		backBufferPtr->Release();

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = myScreenWidth;
		depthStencilDesc.Height = myScreenHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;

		myDevice->CreateTexture2D(&depthStencilDesc, NULL, &myDepthStencilBuffer);
		myDevice->CreateDepthStencilView(myDepthStencilBuffer, &depthStencilViewDesc, &myDepthStencilView);
		myContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthStencilView);
		// ----

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Height = myScreenHeight;
		viewport.Width = myScreenWidth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		myContext->RSSetViewports(1, &viewport);

		D3D11_BUFFER_DESC constBufferDesc;
		constBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constBufferDesc.ByteWidth = sizeof(ConstantBufferObject);
		constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufferDesc.CPUAccessFlags = 0;
		constBufferDesc.MiscFlags = 0;

		myModel.Initialize(myContext, myDevice);
		myModel.SetModel(L"Cube", L"Textures/Jack Daniel.png");

		myDevice->CreateBuffer(&constBufferDesc, NULL, &myConstBufferObjectBuffer);

		myCameraPosition = { 0.0f, 0.0f ,-10.f, 0.0f };
		myCameraTarget = { 1.0f, 0.0f ,0.0f, 0.0f };
		myCameraUp = { 0.0f, 1.0f ,0.0f, 0.0f };

		myCameraView = DirectX::XMMatrixLookAtLH(myCameraPosition, myCameraTarget, myCameraUp);

		myCameraProjection = DirectX::XMMatrixPerspectiveFovLH(0.4f * 3.14f, static_cast<float>(myScreenWidth) / static_cast<float>(myScreenHeight), 0.1f, 1000);
		myWorld = DirectX::XMMatrixIdentity();
		InitPipeline();


	}

	void Engine::InitPipeline()
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		myVertexShader.Initialize(myDevice, L"Shaders/VertexShader.cso", layout, ARRAYSIZE(layout));
		myPixelShader.Initialize(myDevice, L"Shaders/PixelShader.cso");


		myContext->VSSetShader(myVertexShader.GetShader(), 0, 0);
		myContext->PSSetShader(myPixelShader.GetShader(), 0, 0);

		

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
		myDevice->CreateRasterizerState(&cmDesc, &myCCWcullMode);
		cmDesc.FrontCounterClockwise = false;
		myDevice->CreateRasterizerState(&cmDesc, &myCWcullMode);
		
		D3D11_RASTERIZER_DESC rastDesc = {};
		cmDesc.FillMode = D3D11_FILL_SOLID;
		cmDesc.CullMode = D3D11_CULL_NONE;
		myDevice->CreateRasterizerState(&rastDesc, &myNoCull);

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

		myDevice->CreateBlendState(&blendDesc, &myTransparency);


		myContext->IASetInputLayout(myVertexShader.GetInputLayout());
		auto topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		myContext->IAGetPrimitiveTopology(&topology);
	}

	void Engine::Update()
	{
		myRot += 0.0006f;
		if (myRot >= 6.26f)
		{
			myRot = 0.0;
		}

		myCubeWorld = DirectX::XMMatrixIdentity();
		DirectX::XMVECTOR rotaxis = { 1,1,1,0 };
		myRotation = DirectX::XMMatrixRotationAxis(rotaxis, myRot);
		myTranslation = DirectX::XMMatrixTranslation(0, 0, 0.0f);
		myScale = DirectX::XMMatrixScaling(2, 2, 2);
		myCubeWorld = myTranslation * myRotation * myScale;
	}


	void Engine::RenderFrame()
	{
		float color[4] = { 0.5,0.5f, 0.5f, 0.f };
		myContext->ClearRenderTargetView(myRenderTargetView, color);
		myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		float blendFactor[] = { 0.75f, 0.75f,0.75f, 1.f };

		myContext->OMSetBlendState(myTransparency, blendFactor, 0xffffffff);

		myWVP = myCubeWorld * myCameraView * myCameraProjection;
		myConstantBufferObject.worldViewPosition = DirectX::XMMatrixTranspose(myWVP);
		myContext->UpdateSubresource(myConstBufferObjectBuffer, 0, NULL, &myConstantBufferObject, 0, 0);
		myContext->VSSetConstantBuffers(0, 1, &myConstBufferObjectBuffer);
		
		myModel.Draw();

		mySwapchain->Present(0, 0);
	}

	void Engine::CleanD3D() const
	{
		mySwapchain->Release();
		myDepthStencilBuffer->Release();
		myConstBufferObjectBuffer->Release();
		myDevice->Release();
		myRenderTargetView->Release();
		myContext->Release();
	}

}