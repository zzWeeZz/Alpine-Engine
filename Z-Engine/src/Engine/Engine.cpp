#include "Engine.h"
#include "d3d11shader.h"


void Engine::InitD3D(HWND aHWND)
{
	// Inits Direct X
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = aHWND;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapchain,
		&myDev,
		NULL,
		&myDevcon);
	// ---------- 

	// Set Render Target
	ID3D11Texture2D* backBufferPtr;
	mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));

	myDev->CreateRenderTargetView(backBufferPtr, NULL, &myRenderTargetView);
	backBufferPtr->Release();

	myDevcon->OMSetRenderTargets(1, &myRenderTargetView, NULL);
	// ----

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;
	myDevcon->RSSetViewports(1, &viewport);



	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numberOfElements = ARRAYSIZE(layout);
}

void Engine::InitPipeline()
{
}

void Engine::RenderFrame()
{
	float color[5] = { 0,0.2f, 0.5f, 1.f };
	myDevcon->ClearRenderTargetView(myRenderTargetView, color);

	mySwapchain->Present(0, 0);
}

void Engine::CleanD3D()
{
	mySwapchain->Release();
	myVertLayout->Release();
	myVS->Release();
	myPS->Release();
	myPS_Buffer->Release();
	myVS_Buffer->Release();
	myTriangleVertBuffer->Release();
	myDev->Release();
	myRenderTargetView->Release();
	myDevcon->Release();
}
