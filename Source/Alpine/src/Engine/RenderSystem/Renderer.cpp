#include "Renderer.h"
#include "RenderCommands.h"
#include "Application/Application.h"

namespace Alpine
{
	Scope<Stash> s_Stash = std::make_unique<Stash>();
	Renderer Renderer::s_Instance;
	Renderer::Renderer() : m_ModelBuffer(1)
	{
		s_Instance = *this;
	}

	void Renderer::Initalize()
	{
		s_Instance.m_Skybox = SkyBox::Create("Textures/cannon_4k.hdr");
		s_Instance.m_ModelBuffer.Create();
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TBASIS" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		s_Instance.m_PbrVertexShader = VertexShader::Create(L"Shaders/pbrShader_vs.cso", layout, ARRAYSIZE(layout));
		s_Instance.m_PbrPixelShader = PixelShader::Create(L"Shaders/pbrShader_ps.cso");

		D3D11_INPUT_ELEMENT_DESC layout2[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		s_Instance.m_SkyBoxVertexShader = VertexShader::Create(L"Shaders/SkyBox_vs.cso", layout2, ARRAYSIZE(layout2));
		s_Instance.m_SkyBoxPixelShader = PixelShader::Create(L"Shaders/SkyBox_ps.cso");

		FramebufferSpecification spec = {};
		spec.width = Application::GetWindow()->GetWidth();
		spec.height = Application::GetWindow()->GetHeight();

		s_Instance.m_FrameBuffer = FrameBuffer::Create(spec);
	}

	bool Renderer::SubmitMesh(MeshCommand& model)
	{
		s_Stash->meshes.emplace_back(model);
		return true;
	}

	void Renderer::Begin()
	{
		s_Instance.m_FrameBuffer->Bind();
	}

	void Renderer::DrawStash()
	{
		s_Instance.m_PbrVertexShader->Bind();
		s_Instance.m_PbrPixelShader->Bind();
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Pixel, 0);
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Clamp, ShaderType::Pixel, 1);
		for (size_t i = 0; i < s_Stash->meshes.size(); i++)
		{
			s_Instance.m_ModelBuffer.SetData(&s_Stash->meshes[i].transform, sizeof(Matrix));
			s_Instance.m_ModelBuffer.Bind();
			s_Stash->meshes[i].material->Bind();
			s_Stash->meshes[i].mesh.SubmitMesh();
		}

		s_Instance.m_SkyBoxVertexShader->Bind();
		s_Instance.m_SkyBoxPixelShader->Bind();
		s_Instance.m_Skybox->Draw();
	}

	void Renderer::End()
	{
		s_Instance.m_FrameBuffer->ClearView({0,0,0,0});
		s_Instance.m_FrameBuffer->ClearDepthStencil();
	}
}