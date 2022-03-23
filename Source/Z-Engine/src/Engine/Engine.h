#pragma once
#include <d3d11.h>
#include "DirectXMath.h"
#include "Graphics/Shaders.h"
#include "Graphics/Model/Model.h"
#include <wrl.h>

namespace Engine
{
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

		Model myModel;
		VertexShader myVertexShader;
		PixelShader myPixelShader;

		Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapchain;
		Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
		ID3D11DeviceContext* myContext;
		ID3D11RenderTargetView* myRenderTargetView;
		ID3D11DepthStencilView* myDepthStencilView;
		ID3D11Texture2D* myDepthStencilBuffer;
		ID3D11Buffer* myConstBufferObjectBuffer;
		ID3D11RasterizerState* myWireFrame;
		ID3D11RasterizerState* myNoCull;
		ID3D11BlendState* myTransparency;
		ID3D11RasterizerState* myCCWcullMode;
		ID3D11RasterizerState* myCWcullMode;

	};
}