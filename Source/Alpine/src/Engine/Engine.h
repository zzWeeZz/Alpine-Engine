#pragma once
#include <d3d11.h>
#include "Graphics/Shaders.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Buffers/ConstantBuffer.hpp"
#include <wrl.h>
#include "DX11/Utilities.h"

#include "Textures/TextureCube.h"
#include "Camera/PerspectiveCamera.h"
#include "Lights/AmbientLight.h"
#include "imGui/ImGuiLayer.h"
#include "Graphics/Buffers/BufferData.h"
#include "RenderSystem/FrameBuffer.h"

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
		VertexShader m_VertexShader;
		PixelShader m_PixelShader;
		ComputeShader m_IrrComputeShader;
		ComputeShader m_SpecularComputeShader;
		ComputeShader m_SpbrdfShader;

		AmbientLight m_AmbientLight;
		PerspectiveCamera m_Camera;
		Ref<Material> m_MetalicMaterial;
		Ref<Material> m_GroundMaterial;
		
		Ref<Texture> m_Texture;
		
		Ref<TextureCube> m_CubeMap;
		Ref<TextureCube> m_IrMap;
		Ref<TextureCube> m_SpecularMap;
		Ref<FrameBuffer> m_FrameBuffer;

		Model m_MetalMan;
		Model m_RockMan;
		Model m_Ground;
		CameraBuffer m_CameraBufferObject;
		LightBuffer m_LightBufferObject;
		ConstantBuffer<SpectularMapFilerSettingsBuffer> m_SpecBuffer;
		ConstantBuffer<CameraBuffer> m_CameraBuffer;
		ConstantBuffer<Matrix> m_ModelBuffer;
		ConstantBuffer<LightBuffer> m_LightBuffer;

		ImGuiLayer m_ImguiLayer;

		ID3D11RasterizerState* m_WireFrame;
		ID3D11RasterizerState* m_NoCull;
		ID3D11BlendState* m_Transparency;
		ID3D11RasterizerState* m_CCWcullMode;
		ID3D11RasterizerState* m_CWcullMode;

	};
}
