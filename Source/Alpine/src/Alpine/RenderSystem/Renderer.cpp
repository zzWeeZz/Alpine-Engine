#include <iostream>

#include "Renderer.h"
#include "RenderCommands.h"

#include "Application/Application.h"

#include "Alpine/RenderSystem/ShaderClasses/ShaderLibrary.h"

namespace Alpine
{
	static Scope<Data> s_Data = std::make_unique<Data>();

	void Renderer::Initalize()
	{
		s_Data->m_Skybox = SkyBox::Create("Textures/cannon_4k.hdr");
		s_Data->m_ModelBuffer.Create();
		s_Data->m_CameraBuffer.Create();
		s_Data->m_DirLightBuffer.Create();
		s_Data->m_PointLightBuffer.Create();
		

		ShaderLibrary::Store("PBR", { VertexShader::Create(L"Shaders/pbrShader_vs.cso"), PixelShader::Create(L"Shaders/pbrShader_ps.cso") });
		ShaderLibrary::Store("SkyBox", { VertexShader::Create(L"Shaders/SkyBox_vs.cso"), PixelShader::Create(L"Shaders/SkyBox_ps.cso") });


		FramebufferSpecification spec = {};
		spec.width = Application::GetWindow()->GetWidth();
		spec.height = Application::GetWindow()->GetHeight();
		spec.colorFormat = { DXGI_FORMAT_R8G8B8A8_UNORM };

		s_Data->m_FrameBuffer = FrameBuffer::Create(spec);
		auto topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		DX11::Context()->IAGetPrimitiveTopology(&topology);
	}

	void Renderer::SetActiveCamera(Ref<SceneCamera> camera)
	{
		s_Data->camera = camera;
	}

	void Renderer::SetEditorCamera(Ref<EditorCamera> edCamera)
	{
		s_Data->editorCamera = edCamera;
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
		s_Data->m_Skybox->Bind();
		s_Data->m_FrameBuffer->Bind();
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Pixel, 0);
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Clamp, ShaderType::Pixel, 1);
		s_Data->m_ModelBuffer.SetData(&model.transform, sizeof(Matrix));
		s_Data->m_ModelBuffer.Bind();
		DX11::GetRenderStateManager().PushRasterizerState(model.cullmode);
		DX11::GetRenderStateManager().PushDepthStencilState(model.depthStencilMode);
		//model.material->Bind();
		model.mesh.SubmitMesh();
		DX11::GetRenderStateManager().PopRasterizerState();
		DX11::GetRenderStateManager().PopDepthStencilState();
		s_Data->m_FrameBuffer->UnBind();
		return true;
	}

	void Renderer::QueueDraw(Ref<Model> model)
	{
		s_Data->QueuedModel.emplace_back(model);
	}

	void Renderer::SubmitDirLight(DirectionalLight& light)
	{
		if (s_Data->m_DirLightCount == 4)
		{
			std::cout << "Max dir lights reached" << std::endl;
			return;
		}
		s_Data->m_DirLightBufferObject.lightColor = light.GetLightColor();
		s_Data->m_DirLightBufferObject.lightDirection = Vector4(light.GetDirection().x, light.GetDirection().y, light.GetDirection().z, 0.0f);
		s_Data->m_DirLightBuffer.SetData(&s_Data->m_DirLightBufferObject, sizeof(DirectionalLight));
		s_Data->m_DirLightBuffer.Bind();
		s_Data->m_DirLightCount++;
	}

	void Renderer::AddPointLight(PointLight& light)
	{
		if (s_Data->m_PointLightCount == 8)
		{
			std::cout << "Max point lights reached" << std::endl;
			return;
		}
		auto& pointLight = s_Data->m_PointLightBufferObject.PointLightData[s_Data->m_PointLightCount];
		pointLight.lightColor = light.GetLightColor();
		pointLight.lightPosition = Vector4(light.GetPosition().x, light.GetPosition().y, light.GetPosition().z, 1.0f);
		pointLight.lightRange = light.GetRange();
		pointLight.falloff = light.GetFallOff();
		s_Data->m_PointLightBuffer.SetData(&s_Data->m_PointLightBufferObject, sizeof(PointLightBuffer));
		s_Data->m_PointLightBuffer.Bind();
		s_Data->m_PointLightCount++;
	}

	Ref<FrameBuffer> Renderer::GetFrameBuffer()
	{
		return s_Data->m_FrameBuffer;
	}

	void Renderer::Begin()
	{
		s_Data->m_FrameBuffer->ClearView({ 0,0,0,1 });
		s_Data->m_FrameBuffer->ClearDepthStencil();
		s_Data->m_FrameBuffer->Bind();
		s_Data->m_Skybox->Bind();

		s_Data->m_CameraBufferObject.viewMatrix = s_Data->editorCamera->GetViewMatrix();
		s_Data->m_CameraBufferObject.toProjectionSpace = s_Data->editorCamera->GetProjectionMatrix();
		s_Data->m_CameraBufferObject.position = Vector4(s_Data->editorCamera->GetPosition().x, s_Data->editorCamera->GetPosition().y, s_Data->editorCamera->GetPosition().z, 1);
		s_Data->m_CameraBufferObject.toCameraSpace = s_Data->editorCamera->GetViewMatrix().Invert();
		s_Data->m_CameraBuffer.SetData(&s_Data->m_CameraBufferObject, sizeof(CameraBuffer));
		s_Data->m_CameraBuffer.Bind(0, 0);

		ShaderLibrary::Bind("PBR");
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Pixel, 0);
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Clamp, ShaderType::Pixel, 1);
		for (auto& model : s_Data->QueuedModel)
		{
			s_Data->m_ModelBuffer.SetData(&model->GetTransform(), sizeof(Matrix));
			s_Data->m_ModelBuffer.Bind(false, 1);
			model->Draw();
		}

		
		s_Data->m_FrameBuffer->Bind();
		s_Data->m_Skybox->BindForSky();
		s_Data->m_Skybox->Draw();
		s_Data->m_FrameBuffer->UnBind();
	}

	void Renderer::LogDrawCall()
	{
		s_Data->drawCallCount++;
	}

	int Renderer::GetDrawCallCount()
	{
		return s_Data->drawCallCount;
	}
}