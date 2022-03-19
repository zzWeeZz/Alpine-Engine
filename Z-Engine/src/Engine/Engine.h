#pragma once
#include <d3d11.h>
#include "DirectXMath.h"

struct Vertex    //Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z, float r, float g, float b, float a)
		: pos(x, y, z), color(r, g, b, a) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

class Engine
{
public:
	void InitD3D(HWND aHWND, int aScreenWidth, int aScreenHight);
	void InitPipeline();
	void RenderFrame();
	void CleanD3D() const;
	void DrawScene();
	
private:

	int myScreenWidth;
	int myScreenHeight;


	IDXGISwapChain* mySwapchain;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	ID3D11RenderTargetView* myRenderTargetView;
	ID3D11Buffer* mySquareIndexBuffer;
	ID3D11Buffer* mySquareVertBuffer;
	ID3D11DepthStencilView* myDepthStencilView;
	ID3D11Texture2D* myDepthStencilBuffer;
	ID3D11VertexShader* myVS;
	ID3D11PixelShader* myPS;
	ID3D10Blob* myVS_Buffer;
	ID3D10Blob* myPS_Buffer;
	ID3D11InputLayout* myVertLayout;


};