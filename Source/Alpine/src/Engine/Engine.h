#pragma once
#include <d3d11.h>
#include "Graphics/Shaders.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Buffers/ConstantBuffer.hpp"
#include <wrl.h>
#include <DirectXTK/SimpleMath.h>

#include "Textures/TextureCube.h"
using namespace DirectX::SimpleMath;
#include "Camera/PerspectiveCamera.h"
#include "Lights/AmbientLight.h"
#include "imGui/ImGuiLayer.h"
#include "Graphics/Buffers/BufferData.h"

namespace Alpine
{
	class Engine
	{
	public:
		Engine();
		void InitD3D(HWND aHWND, int aScreenWidth, int aScreenHight);
		void InitObjects();
		void Update(float aDeltaTime);
		void RenderFrame();
		void CleanD3D();
	private:
		VertexShader myVertexShader;
		PixelShader myPixelShader;
		ComputeShader myComputeShader;

		AmbientLight myAmbientLight;
		PerspectiveCamera myCamera;
		std::shared_ptr<Material> myMetalicMaterial;
		std::shared_ptr<Material> myGroundMaterial;
		std::shared_ptr<Texture> myTexture;
		std::shared_ptr<TextureCube> myCubeMap;
		ID3D11UnorderedAccessView* myUAV;
		Model myHeli;
		Model mySphere;
		Model myGround;
		CameraBuffer myCameraBufferObject;
		LightBuffer myLightBufferObject;
		ConstantBuffer<CameraBuffer> myCameraBuffer;
		ConstantBuffer<Matrix> myModelBuffer;
		ConstantBuffer<LightBuffer> myLightBuffer;

		ImGuiLayer myImguiLayer;

		ID3D11RasterizerState* myWireFrame;
		ID3D11RasterizerState* myNoCull;
		ID3D11BlendState* myTransparency;
		ID3D11RasterizerState* myCCWcullMode;
		ID3D11RasterizerState* myCWcullMode;

	};
}
