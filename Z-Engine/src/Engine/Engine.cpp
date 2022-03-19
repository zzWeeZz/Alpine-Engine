#include "Engine.h"

#include <windows.h>
#include <d3d11.h>

#include <d3dcompiler.h>


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

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

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
		Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
	};

	DWORD index[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	//The input-layout description
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElement = ARRAYSIZE(layout);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = index;
	myDevice->CreateBuffer(&indexBufferDesc, &iinitData, &mySquareIndexBuffer);
	myContext->IASetIndexBuffer(mySquareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexBufferData;

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


void Engine::RenderFrame()
{
	float color[4] = { 0.5,0.5f, 0.5f, 1.f };
	myContext->ClearRenderTargetView(myRenderTargetView, color);
	myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	myContext->DrawIndexed(6, 0, 0);
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
	myDevice->Release();
	myRenderTargetView->Release();
	myContext->Release();
}

void Engine::DrawScene()
{

	float bgColor[4] = { (0.0f, 0.0f, 0.0f, 0.0f) };
	myContext->ClearRenderTargetView(myRenderTargetView, bgColor);

	myContext->Draw(3, 0);

	mySwapchain->Present(0, 0);

}
