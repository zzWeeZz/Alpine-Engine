#include "Renderer.h"
#include "RenderCommands.h"
#include "Application/Application.h"

namespace Alpine
{
	static Scope<Stash> s_Stash = std::make_unique<Stash>();
	Renderer Renderer::s_Instance;
	Renderer::Renderer() : m_CameraBuffer(0), m_ModelBuffer(1), m_LightBuffer(2)
	{
		s_Instance = *this;
	}

	void Renderer::Initalize()
	{
		s_Instance.m_Skybox = SkyBox::Create("Textures/monbachtal_riverbank_4k.hdr");
		s_Instance.m_ModelBuffer.Create();
		s_Instance.m_CameraBuffer.Create();
		s_Instance.m_LightBuffer.Create();
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
		auto topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		DX11::Context()->IAGetPrimitiveTopology(&topology);
	}

	void Renderer::SubmitCamera(Ref<PerspectiveCamera> camera)
	{
		s_Stash->camera = camera;
	}

	bool Renderer::SubmitMesh(MeshCommand& model)
	{
		if(model.cullmode == CullMode::Front)
		{
			s_Instance.m_SkyBoxVertexShader->Bind();
			s_Instance.m_SkyBoxPixelShader->Bind();
		}
		else
		{
			s_Instance.m_PbrVertexShader->Bind();
			s_Instance.m_PbrPixelShader->Bind();
		}
		s_Instance.m_Skybox->Bind();
		s_Instance.m_FrameBuffer->Bind();
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Pixel, 0);
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Clamp, ShaderType::Pixel, 1);
		s_Instance.m_ModelBuffer.SetData(&model.transform, sizeof(Matrix));
		s_Instance.m_ModelBuffer.Bind();
		DX11::GetRenderStateManager().PushRasterizerState(model.cullmode);
		DX11::GetRenderStateManager().PushDepthStencilState(model.depthStencilMode);
		model.material->Bind();
		model.mesh.SubmitMesh();
		DX11::GetRenderStateManager().PopRasterizerState();
		DX11::GetRenderStateManager().PopDepthStencilState();
		s_Instance.m_FrameBuffer->UnBind();
		return true;
	}
	Ref<FrameBuffer> Renderer::GetFrameBuffer()
	{
		return s_Instance.m_FrameBuffer;
	}
	void Renderer::Begin()
	{
		s_Instance.m_FrameBuffer->ClearView({ 0,1,0,1 });
		s_Instance.m_FrameBuffer->ClearDepthStencil();
		s_Instance.m_FrameBuffer->Bind();
		s_Instance.m_Skybox->BindForSky();
		s_Instance.m_Skybox->Draw();
		s_Instance.m_FrameBuffer->UnBind();
	}

	void Renderer::DrawStash()
	{
		s_Instance.m_CameraBufferObject.position = Vector4(s_Stash->camera->GetPosition().x, s_Stash->camera->GetPosition().y, s_Stash->camera->GetPosition().z, 1);
		s_Instance.m_CameraBufferObject.toCameraSpace = s_Stash->camera->GetViewMatrix();
		s_Instance.m_CameraBufferObject.toProjectionSpace = s_Stash->camera->GetProjectionMatrix();
		s_Instance.m_CameraBufferObject.viewMatrix = s_Stash->camera->GetViewMatrix();
		s_Instance.m_CameraBuffer.SetData(&s_Instance.m_CameraBufferObject, sizeof(CameraBuffer));
		s_Instance.m_CameraBuffer.Bind();

		s_Instance.m_LightBufferObject.ambientColor = Vector4(0.1f, 0.1f, 0.1f, 1);
		s_Instance.m_LightBuffer.SetData(&s_Instance.m_LightBufferObject, sizeof(LightBuffer));
		s_Instance.m_LightBuffer.Bind();
	}

	void Renderer::End()
	{
		s_Instance.m_FrameBuffer->UnBind();
	}
}