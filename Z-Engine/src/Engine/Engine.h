#pragma once
#include <d3d11.h>

class Engine
{
public:
	void InitD3D(HWND aHWND);
	void InitPipeline();
	void RenderFrame();
	void CleanD3D();
private:
	IDXGISwapChain* mySwapchain;           
	ID3D11Device* myDev;                   
	ID3D11DeviceContext* myDevcon;
	ID3D11RenderTargetView* myBackBuffer;
};