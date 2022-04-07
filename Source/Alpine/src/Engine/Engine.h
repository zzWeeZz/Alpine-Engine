#pragma once
#include <d3d11.h>
#include "Graphics/Shaders.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Buffers/ConstantBuffer.hpp"
#include <wrl.h>
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;
#include "Graphics/Camera.h"
#include "Lights/AmbientLight.h"
#include "imGui/ImGuiLayer.h"

namespace Alpine
{
	struct CameraConstBuffer
	{
		Matrix toCameraSpace;
		Matrix toProjectionSpace;
	};
	struct DynamicLightBuffer
	{
		Vector4 lightDirection;
		Vector4 lightColor;
	};
	struct LightConstBuffer
	{
		DynamicLightBuffer lights[16];
		Vector4 ambientColor;
	};

	class Engine
	{
	public:
		Engine();
		void InitD3D(HWND aHWND, int aScreenWidth, int aScreenHight);
		void InitPipeline();
		void Update(float aDeltaTime);
		void RenderFrame();
		void CleanD3D();
	private:
		int myScreenWidth;
		int myScreenHeight;

		VertexShader myVertexShader;
		PixelShader myPixelShader;

		AmbientLight myAmbientLight;
		Camera myCamera;		
		Model myModel;
		Model myLambo;
		Model myGround;
		CameraConstBuffer myConstantBufferObject;
		LightConstBuffer myLightConstantBufferObject;
		ConstantBuffer<CameraConstBuffer> myConstantBuffer;
		ConstantBuffer<Matrix> myModelBuffer;
		ConstantBuffer<LightConstBuffer> myLightBuffer;

		ImGuiLayer myImguiLayer;

		ID3D11Texture2D* myDepthStencilBuffer;
		ID3D11Buffer* myConstBufferObjectBuffer;
		ID3D11RasterizerState* myWireFrame;
		ID3D11RasterizerState* myNoCull;
		ID3D11BlendState* myTransparency;
		ID3D11RasterizerState* myCCWcullMode;
		ID3D11RasterizerState* myCWcullMode;

	};
}
