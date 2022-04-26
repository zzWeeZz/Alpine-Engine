#pragma once
#include <vector>
#include "DX11/Utilities.h"
#include "Graphics/Buffers/ConstantBuffer.hpp"
#include "FrameBuffer.h"

#include "Graphics/Shaders.h"

#include "Graphics/SkyBox/SkyBox.h"
namespace Alpine
{
	class MeshCommand;
	
	struct Stash
	{
		std::vector<MeshCommand> meshes;
	};

	class Renderer
	{
	public:
		Renderer();
		static void Initalize();
		static bool SubmitMesh(MeshCommand& model);
		static void Begin();
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
	};
}

