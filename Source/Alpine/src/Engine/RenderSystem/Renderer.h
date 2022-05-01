#pragma once
#include <vector>
#include "Engine/DX11/Utilities.h"
#include "Engine/Graphics/Buffers/ConstantBuffer.hpp"
#include "Engine/Camera/PerspectiveCamera.h"
#include "FrameBuffer.h"

#include "Engine/Graphics/Shaders.h"
#include "Engine/Lights/DirectionalLight.h"
#include "Engine/Lights/PointLight.h"
#include "Engine/Graphics/SkyBox/SkyBox.h"
namespace Alpine
{
	class MeshCommand;
	
	struct Stash
	{
		Ref<PerspectiveCamera> camera;
		std::vector<MeshCommand> meshes;
	};

	class Renderer
	{
	public:
		Renderer();
		static void Initalize();
		static void SubmitCamera(Ref<PerspectiveCamera> camera);
		static bool SubmitMesh(MeshCommand& model);
		static void SubmitDirLight(DirectionalLight& light);
		static void AddPointLight(PointLight& light);
		static void Begin();
		static Ref<FrameBuffer> GetFrameBuffer();
		static void DrawStash();
		static void End();
	private:
		static Renderer s_Instance;
		Ref<SkyBox> m_Skybox;
		Ref<VertexShader> m_PbrVertexShader;
		Ref<PixelShader> m_PbrPixelShader;

		Ref<VertexShader> m_SkyBoxVertexShader;
		Ref<PixelShader> m_SkyBoxPixelShader;

		Ref<FrameBuffer> m_FrameBuffer;
		ConstantBuffer<Matrix> m_ModelBuffer;

		CameraBuffer m_CameraBufferObject;
		ConstantBuffer<CameraBuffer> m_CameraBuffer;

		DirLightBuffer m_DirLightBufferObject;
		ConstantBuffer<DirLightBuffer> m_DirLightBuffer;

		size_t m_PointLightCount = 0;
		PointLightBuffer m_PointLightBufferObject;
		ConstantBuffer<PointLightBuffer> m_PointLightBuffer;
	};
}

