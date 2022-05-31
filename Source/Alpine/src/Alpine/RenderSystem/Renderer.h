#pragma once
#include <vector>
#include <utility>
#include "FrameBuffer.h"
#include "Alpine/Assets/Camera/EditorCamera.h"

#include "Alpine/DX11/Utilities.h"

#include "Alpine/RenderSystem/Shaders.h"
#include "Alpine/RenderSystem/Mesh/ModelData.hpp"
#include "Alpine/RenderSystem/Buffers/BufferData.h"
#include "Alpine/RenderSystem/Buffers/ConstantBuffer.hpp"
#include "Alpine/RenderSystem/Buffers/VertexBuffer.h"
#include "Alpine/RenderSystem/Buffers/IndexBuffer.h"

#include "Alpine/Assets/SkyBox/SkyBox.h"
#include "Alpine/Assets/Lights/PointLight.h"
#include "Alpine/Assets/Lights/DirectionalLight.h"
#include "Alpine/Assets/Camera/SceneCamera.h"


namespace Alpine
{
	class MeshCommand;
	
	struct Stash
	{
		Ref<SceneCamera> camera;
		Ref<EditorCamera> editorCamera;
		std::vector<MeshCommand> meshes;

		int drawCallCount = 0;
	};

	class Renderer
	{
	public:
		Renderer();
		static void Initalize();
		static void SubmitCamera(Ref<SceneCamera> camera);
		static void SubmitEditorCamera(Ref<EditorCamera> edCamera);
		static bool SubmitMesh(MeshCommand& model);
		static void SubmitDirLight(DirectionalLight& light);
		static void AddPointLight(PointLight& light);
		static void Begin();
		static void LogDrawCall();
		static float GetDrawCallCount();
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

		size_t m_DirLightCount = 0;
		DirLightBuffer m_DirLightBufferObject;
		ConstantBuffer<DirLightBuffer> m_DirLightBuffer;

		size_t m_PointLightCount = 0;
		PointLightBuffer m_PointLightBufferObject;
		ConstantBuffer<PointLightBuffer> m_PointLightBuffer;
	};
}

