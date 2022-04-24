#pragma once
#include <d3d11.h>
#include "Graphics/Shaders.h"
#include "Graphics/Model/Model.h"
#include "DX11/Utilities.h"

#include "Textures/TextureCube.h"
#include "Camera/PerspectiveCamera.h"
#include "Lights/AmbientLight.h"
#include "imGui/ImGuiLayer.h"
#include "Graphics/Buffers/BufferData.h"
#include "RenderSystem/FrameBuffer.h"
#include "Graphics/SkyBox/SkyBox.h"

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

		AmbientLight m_AmbientLight;
		PerspectiveCamera m_Camera;
		Ref<Material> m_MetalicMaterial;
		Ref<Material> m_GroundMaterial;

		Ref<FrameBuffer> m_FrameBuffer;
		Ref<SkyBox> m_SkyBox;
		Model m_MetalMan;
		Model m_RockMan;
		CameraBuffer m_CameraBufferObject;
		LightBuffer m_LightBufferObject;
		ConstantBuffer<CameraBuffer> m_CameraBuffer;
		ConstantBuffer<Matrix> m_ModelBuffer;
		ConstantBuffer<LightBuffer> m_LightBuffer;

		ImGuiLayer m_ImguiLayer;

		

	};
}
