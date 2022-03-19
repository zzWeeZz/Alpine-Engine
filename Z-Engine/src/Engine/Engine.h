#pragma once
#include <d3d11.h>
#include "DirectXMath.h"

struct Vertex    //Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
};


struct ConstantBufferObject
{
	DirectX::XMMATRIX worldViewPosition;
};

class Engine
{
public:
	void InitD3D(HWND aHWND, int aScreenWidth, int aScreenHight);
	void InitPipeline();
	void Update();
	void RenderFrame();
	void CleanD3D() const;
private:

	bool myBool;

	int myScreenWidth;
	int myScreenHeight;

	ConstantBufferObject myConstantBufferObject;

	DirectX::XMMATRIX myWVP;
	DirectX::XMMATRIX myCubeWorld;

	DirectX::XMMATRIX myWorld;
	DirectX::XMMATRIX myCameraView;
	DirectX::XMMATRIX myCameraProjection;

	DirectX::XMVECTOR myCameraPosition;
	DirectX::XMVECTOR myCameraTarget;
	DirectX::XMVECTOR myCameraUp;

	DirectX::XMMATRIX myRotation;
	DirectX::XMMATRIX myScale;
	DirectX::XMMATRIX myTranslation;
	float myRot = 0.01f;

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
	ID3D11Buffer* myConstBufferObjectBuffer;
	ID3D11RasterizerState* myWireFrame;
	ID3D11RasterizerState* myNoCull;
	ID3D11ShaderResourceView* myCubesTexture;
	ID3D11SamplerState* myCubesTexSamplerState;
	ID3D11BlendState* myTransparency;
	ID3D11RasterizerState* myCCWcullMode;
	ID3D11RasterizerState* myCWcullMode;

};