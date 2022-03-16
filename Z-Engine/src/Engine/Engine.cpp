#include "Engine.h"


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

	myDev->CreateRenderTargetView(backBufferPtr, NULL, &myBackBuffer);
	backBufferPtr->Release();

	myDevcon->OMSetRenderTargets(1, &myBackBuffer, NULL);
	// ----

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	myDevcon->RSSetViewports(1, &viewport);
}

void Engine::InitPipeline()
{
	ID3D10Blob* VS, * PS;
}

void Engine::RenderFrame()
{
	float color[5] = { 0,0.2f, 0.5f, 1.f };
	myDevcon->ClearRenderTargetView(myBackBuffer, color);

	mySwapchain->Present(0, 0);
}

void Engine::CleanD3D()
{
	mySwapchain->Release();
	myDev->Release();
	myBackBuffer->Release();
	myDevcon->Release();
}
