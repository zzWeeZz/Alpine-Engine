#pragma once
#include <d3d11.h>
#include "Graphics/Shaders.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Buffers/ConstantBuffer.hpp"
#include <wrl.h>
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;
#include "Graphics/Camera.h"

namespace Engine
{
	struct ConstantBufferValues
	{
		Matrix modelSpace;
		Matrix toCameraSpace;
		Matrix toProjectionSpace;
	};

	class Engine
	{
	public:
		void InitD3D(HWND aHWND, int aScreenWidth, int aScreenHight);
		void InitPipeline();
		void Update(float aDeltaTime);
		void RenderFrame();
		void CleanD3D() const;
	private:

		bool myBool;

		int myScreenWidth;
		int myScreenHeight;

		ConstantBufferValues myConstantBufferObject;
		Camera myCamera;
		float myRot = 0.01f;
		Model myModel;
		VertexShader myVertexShader;
		PixelShader myPixelShader;
		ConstantBuffer<ConstantBufferValues> myConstantBuffer;
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
