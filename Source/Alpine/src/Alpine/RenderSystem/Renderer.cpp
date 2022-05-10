#include <iostream>

#include "Renderer.h"
#include "RenderCommands.h"

#include "Application/Application.h"

#include "Alpine/RenderSystem/ShaderClasses/ShaderLibrary.h"

namespace Alpine
{
	static Scope<Stash> s_Stash = std::make_unique<Stash>();
	Renderer Renderer::s_Instance;
	Renderer::Renderer() : m_CameraBuffer(0), m_ModelBuffer(1), m_DirLightBuffer(2), m_PointLightBuffer(3)
	{
		s_Instance = *this;
	}

	void Renderer::Initalize()
	{
		s_Instance.m_Skybox = SkyBox::Create("Textures/cannon_4k.hdr");
		s_Instance.m_ModelBuffer.Create();
		s_Instance.m_CameraBuffer.Create();
		s_Instance.m_DirLightBuffer.Create();
		s_Instance.m_PointLightBuffer.Create();
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
		ShaderLibrary::Store("PBR", { s_Instance.m_PbrVertexShader, s_Instance.m_PbrPixelShader });
		D3D11_INPUT_ELEMENT_DESC layout2[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		s_Instance.m_SkyBoxVertexShader = VertexShader::Create(L"Shaders/SkyBox_vs.cso", layout2, ARRAYSIZE(layout2));
		s_Instance.m_SkyBoxPixelShader = PixelShader::Create(L"Shaders/SkyBox_ps.cso");
		ShaderLibrary::Store("SkyBox", { s_Instance.m_SkyBoxVertexShader, s_Instance.m_SkyBoxPixelShader });
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
		if (model.cullmode == CullMode::Front)
		{
			ShaderLibrary::Bind("SkyBox");
		}
		else
		{
			ShaderLibrary::Bind("PBR");
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

	void Renderer::SubmitDirLight(DirectionalLight& light)
	{
		s_Instance.m_DirLightBufferObject.lightColor = light.GetLightColor();
		s_Instance.m_DirLightBufferObject.lightDirection = Vector4(light.GetDirection().x, light.GetDirection().y, light.GetDirection().z, 0.0f);
		s_Instance.m_DirLightBuffer.SetData(&s_Instance.m_DirLightBufferObject, sizeof(DirectionalLight));
		s_Instance.m_DirLightBuffer.Bind();
	}

	void Renderer::AddPointLight(PointLight& light)
	{
		if(s_Instance.m_PointLightCount == 8)
		{
			std::cout << "Max point lights reached" << std::endl;
			return;
		}
		auto& pointLight = s_Instance.m_PointLightBufferObject.PointLightData[s_Instance.m_PointLightCount];
		pointLight.lightColor = light.GetLightColor();
		pointLight.lightPosition = Vector4(light.GetPosition().x, light.GetPosition().y, light.GetPosition().z, 1.0f);
		pointLight.lightRange = light.GetRange();
		pointLight.falloff = light.GetFallOff();
		s_Instance.m_PointLightBuffer.SetData(&s_Instance.m_PointLightBufferObject, sizeof(PointLightBuffer));
		s_Instance.m_PointLightBuffer.Bind();
		s_Instance.m_PointLightCount++;
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
	}

	void Renderer::End()
	{
		s_Instance.m_FrameBuffer->UnBind();
	}
}