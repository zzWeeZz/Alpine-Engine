#include "Engine.h"

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXTK/WICTextureLoader.h>
<<<<<<< HEAD:Source/Z-Engine/src/Engine/Engine.cpp
#include <assimp/aabb.h>

=======
>>>>>>> parent of c8f61cc (assimp not working):Z-Engine/src/Engine/Engine.cpp

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

	DirectX::CreateWICTextureFromFile(myDevice, L"Textures/sus.png", nullptr, &myCubesTexture);


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

	

	myDevice->CreateBuffer(&constBufferDesc, NULL, &myConstBufferObjectBuffer);

	myCameraPosition = { 0.0f, 2.0f ,-20.f, 0.0f };
	myCameraTarget = { 0.0f, 0.0f ,0.0f, 0.0f };
	myCameraUp = { 0.0f, 1.0f ,0.0f, 0.0f };

	myCameraView = DirectX::XMMatrixLookAtLH(myCameraPosition, myCameraTarget, myCameraUp);

	myCameraProjection = DirectX::XMMatrixPerspectiveFovLH(0.4f * 3.14f, static_cast<float>(myScreenWidth) / static_cast<float>(myScreenHeight), 0.1f, 1000);
	myWorld = DirectX::XMMatrixIdentity();
	InitPipeline();


}

void Engine::InitPipeline()
{

	ID3D10Blob* errorMessages;
	auto hr = D3DCompileFromFile(L"Effects.fx", NULL, NULL, "VS", "vs_5_0", 0, 0, &myVS_Buffer, &errorMessages);
	hr = D3DCompileFromFile(L"Effects.fx", NULL, NULL, "PS", "ps_5_0", 0, 0, &myPS_Buffer, &errorMessages);
	//Create the Shader Objects
	hr = myDevice->CreateVertexShader(myVS_Buffer->GetBufferPointer(), myVS_Buffer->GetBufferSize(), NULL, &myVS);
	hr = myDevice->CreatePixelShader(myPS_Buffer->GetBufferPointer(), myPS_Buffer->GetBufferSize(), NULL, &myPS);

	myContext->VSSetShader(myVS, 0, 0);
	myContext->PSSetShader(myPS, 0, 0);

	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};

	DWORD index[] = 
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	myDevice->CreateSamplerState(&samplerDesc, &myCubesTexSamplerState);
	//The input-layout description
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElement = ARRAYSIZE(layout);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = index;
	myDevice->CreateBuffer(&indexBufferDesc, &iinitData, &mySquareIndexBuffer);
	myContext->IASetIndexBuffer(mySquareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexBufferData;

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



	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = myDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mySquareVertBuffer);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	myContext->IASetVertexBuffers(0, 1, &mySquareVertBuffer, &stride, &offset);
	hr = myDevice->CreateInputLayout(layout, numElement, myVS_Buffer->GetBufferPointer(), myVS_Buffer->GetBufferSize(), &myVertLayout);

	myContext->IASetInputLayout(myVertLayout);
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

	DirectX::XMVECTOR cubePos = DirectX::XMVectorZero();

	myWVP = myCubeWorld * myCameraView * myCameraProjection;
	myConstantBufferObject.worldViewPosition = DirectX::XMMatrixTranspose(myWVP);
	myContext->UpdateSubresource(myConstBufferObjectBuffer, 0, NULL, &myConstantBufferObject, 0, 0);
	myContext->VSSetConstantBuffers(0, 1, &myConstBufferObjectBuffer);
	myContext->PSSetShaderResources(0, 1, &myCubesTexture);
	myContext->PSSetSamplers(0, 1, &myCubesTexSamplerState);
	myContext->RSSetState(myCCWcullMode);
	myContext->DrawIndexed(36, 0, 0);
	myContext->RSSetState(myCWcullMode);
	myContext->DrawIndexed(36, 0, 0);


	mySwapchain->Present(0, 0);
}

void Engine::CleanD3D() const
{
	mySwapchain->Release();
	myVertLayout->Release();
	myVS->Release();
	myVS_Buffer->Release();
	mySquareIndexBuffer->Release();
	mySquareVertBuffer->Release();
	myDepthStencilBuffer->Release();
	myConstBufferObjectBuffer->Release();
	myDevice->Release();
	myRenderTargetView->Release();
	myContext->Release();
}
