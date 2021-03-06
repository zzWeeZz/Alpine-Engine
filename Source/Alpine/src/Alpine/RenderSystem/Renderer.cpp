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
		s_Data->m_Skybox = SkyBox::Create("Textures/moonless_golf_4k.hdr");
		s_Data->m_ModelBuffer.Create();
		s_Data->m_CameraBuffer.Create();
		s_Data->m_DirLightBuffer.Create();
		s_Data->m_PointLightBuffer.Create();

		ShaderLibrary::Store("PBR", { VertexShader::Create(L"Shaders/pbrShader_vs.cso"), PixelShader::Create(L"Shaders/pbrShader_ps.cso") });
		ShaderLibrary::Store("SkyBox", { VertexShader::Create(L"Shaders/SkyBox_vs.cso"), PixelShader::Create(L"Shaders/SkyBox_ps.cso") });
		ShaderLibrary::Store("Shadow", {VertexShader::Create("Shaders/ShadowPass_vs.cso"), PixelShader::Create("Shaders/ShadowPass_ps.cso")});
		{
			FramebufferSpecification spec = {};
			spec.width = 2048;
			spec.height = 2048;
			spec.colorFormat = { DXGI_FORMAT_R8_UNORM, DXGI_FORMAT_R32_TYPELESS};
			s_Data->shadowPass.frameBuffer = FrameBuffer::Create(spec);
			s_Data->shadowPass.ShaderKey = "Shadow";
		}
		{
			FramebufferSpecification spec = {};
			spec.width = Application::GetWindow()->GetWidth();
			spec.height = Application::GetWindow()->GetHeight();
			spec.colorFormat = { DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R32_TYPELESS };

			s_Data->m_FrameBuffer = FrameBuffer::Create(spec);
		}
		auto topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		DX11::Context()->IASetPrimitiveTopology(topology);
		
	}

	void Renderer::SetActiveCamera(Ref<SceneCamera> camera)
	{
		s_Data->camera = camera;
	}

	void Renderer::SetEditorCamera(Ref<EditorCamera> edCamera)
	{
		s_Data->editorCamera = edCamera;
	}


	void Renderer::QueueDraw(Ref<Model> model)
	{
		s_Data->QueuedModel.emplace_back(model);
	}

	void Renderer::SubmitDirLight(DirectionalLight& light)
	{
		if (s_Data->m_DirLightCount == MAX_DIR_LIGHTS)
		{
			spdlog::warn("Max Dir light reached!");
			return;
		}
		auto& pointLight = s_Data->m_DirLightBufferObject.DirLightData[s_Data->m_DirLightCount];
		pointLight.lightColor = light.GetLightColor();
		pointLight.lightDirection = Vector4(light.GetDirection().x, light.GetDirection().y, light.GetDirection().z, 0.0f);
		pointLight.lightSpaceMatrix = light.GetLightSpaceMatrix();
		s_Data->m_DirLightCount++;
	}

	void Renderer::AddPointLight(PointLight& light)
	{
		if (s_Data->m_PointLightCount == MAX_POINT_LIGHTS)
		{
			std::cout << "Max point lights reached" << std::endl;
			return;
		}
		auto& pointLight = s_Data->m_PointLightBufferObject.PointLightData[s_Data->m_PointLightCount];
		pointLight.lightColor = light.GetLightColor();
		pointLight.lightPosition = Vector4(light.GetPosition().x, light.GetPosition().y, light.GetPosition().z, 1.0f);
		pointLight.lightRange = light.GetRange();
		pointLight.falloff = light.GetFallOff();
		s_Data->m_PointLightCount++;
	}

	Ref<FrameBuffer> Renderer::GetFrameBuffer()
	{
		return s_Data->m_FrameBuffer;
	}

	void Renderer::Shutdown()
	{
	}

	void ResetDataContainer(Scope<Data>& data)
	{
		data->debugData.LastDrawCallCount = data->drawCallCount;
		data->drawCallCount = 0;
		data->debugData.LastDirLightCount = data->m_DirLightCount;
		data->m_DirLightCount = 0;
		data->debugData.LastPointLightCount = data->m_PointLightCount;
		data->m_PointLightCount = 0;
		data->QueuedModel.clear();
	}

	void Renderer::Begin()
	{
		s_Data->m_FrameBuffer->ClearView({ 0,0,0,1 });
		s_Data->m_FrameBuffer->ClearDepthStencil();
		s_Data->shadowPass.frameBuffer->ClearDepthStencil();
		s_Data->shadowPass.frameBuffer->ClearView({ 1,1,1,1 });
		// bind Lights:
		// bind Direction light
		s_Data->m_DirLightBuffer.SetData(&s_Data->m_DirLightBufferObject, sizeof(DirLightBuffer));
		s_Data->m_DirLightBuffer.Bind(false, 2);

		// bind Point light
		s_Data->m_PointLightBuffer.SetData(&s_Data->m_PointLightBufferObject, sizeof(PointLightBuffer));
		s_Data->m_PointLightBuffer.Bind(false, 3);
		s_Data->shadowPass.frameBuffer->Bind();
		ShaderLibrary::Bind(s_Data->shadowPass.ShaderKey);
		DX11::GetRenderStateManager().PushRasterizerState(CullMode::Front);
		for (auto model : s_Data->QueuedModel)
		{
			s_Data->m_ModelBuffer.SetData(&model->GetTransform(), sizeof(Matrix));
			s_Data->m_ModelBuffer.Bind(false, 1);
			model->Draw(false);
		}
		s_Data->shadowPass.frameBuffer->UnBind();
		s_Data->shadowPass.frameBuffer->BindForShader(3);
		s_Data->m_FrameBuffer->Bind();


		// bind camera
		s_Data->m_CameraBufferObject.viewMatrix = s_Data->editorCamera->GetViewMatrix();
		s_Data->m_CameraBufferObject.toProjectionSpace = s_Data->editorCamera->GetProjectionMatrix();
		s_Data->m_CameraBufferObject.position = Vector4(s_Data->editorCamera->GetPosition().x, s_Data->editorCamera->GetPosition().y, s_Data->editorCamera->GetPosition().z, 1);
		s_Data->m_CameraBufferObject.toCameraSpace = s_Data->editorCamera->GetViewMatrix();
		s_Data->m_CameraBuffer.SetData(&s_Data->m_CameraBufferObject, sizeof(CameraBuffer));
		s_Data->m_CameraBuffer.Bind(false, 0);

		ShaderLibrary::Bind("SkyBox");

		s_Data->m_Skybox->BindForSky();
		s_Data->m_ModelBuffer.SetData(&s_Data->m_Skybox->m_Model->GetTransform(), sizeof(Matrix));
		s_Data->m_ModelBuffer.Bind(false, 1);
		
		DX11::GetRenderStateManager().PushDepthStencilState(DepthStencilMode::ReadOnly);
		s_Data->m_Skybox->Draw();
		DX11::GetRenderStateManager().PopRasterizerState();
		DX11::GetRenderStateManager().PopDepthStencilState();
		s_Data->m_Skybox->Bind();
		

		ShaderLibrary::Bind("PBR");
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Pixel, 0);
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Point, ShaderType::Pixel, 1);
		for (auto model : s_Data->QueuedModel)
		{
			s_Data->m_ModelBuffer.SetData(&model->GetTransform(), sizeof(Matrix));
			s_Data->m_ModelBuffer.Bind(false, 1);
			model->Draw();
		}
		s_Data->m_FrameBuffer->UnBind();
	
		ResetDataContainer(s_Data);
	}

	void Renderer::LogDrawCall()
	{
		s_Data->drawCallCount++;
	}

	int Renderer::GetDrawCallCount()
	{
		return s_Data->debugData.LastDrawCallCount;
	}

	int Renderer::GetDirlightCount()
	{
		return s_Data->debugData.LastDirLightCount;
	}

	int Renderer::GetPointLightCount()
	{
		return s_Data->debugData.LastPointLightCount;
	}
}
