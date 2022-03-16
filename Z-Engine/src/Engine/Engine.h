#pragma once
#include <d3d11.h>
#include "DirectXMath.h"

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

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
	ID3D11RenderTargetView* myRenderTargetView;
	ID3D11Buffer* myTriangleVertBuffer;
	ID3D11VertexShader* myVS;
	ID3D11PixelShader* myPS;
	ID3D10Blob* myVS_Buffer;
	ID3D10Blob* myPS_Buffer;
	ID3D11InputLayout* myVertLayout;
};