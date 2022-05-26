#include "Scene.h"
#include "Entity.h"
#include "DefaultComponents.h"
#include "ToolBox/Utility/Chrono.h"

Alpine::Scene::Scene() : m_Manager(Snowflake::GetManager())
{
}

void Alpine::Scene::Start()
{
}

void Alpine::Scene::OnUpdate()
{
	m_Manager.Execute<MeshComponent, TransformComponent>([](Snowflake::Entity entity, MeshComponent& mesh, TransformComponent& transform)
		{
			if (!mesh.model)
			{
				mesh.model = Model::Create(mesh.MeshPath.string());
			}
			mesh.model->SetPosition(transform.position);
			mesh.model->SetRotation(transform.rotation);
			mesh.model->SetScale(transform.size);
		});

	m_Manager.Execute<PointLightComponent, TransformComponent>([](Snowflake::Entity entity, PointLightComponent& light, TransformComponent& transform)
		{
			if (!light.light)
			{
				light.light = PointLight::Create();
			}
			light.light->SetPosition(transform.position);
			light.light->SetFallOff(light.Falloff);
			light.light->SetRange(light.Range);
			light.light->SetLightColor(Vector4(light.Color.x, light.Color.y, light.Color.z, light.Intensity));
		});
	m_Manager.Execute<DirectionalLightComponent, TransformComponent>([](Snowflake::Entity entity, DirectionalLightComponent& light, TransformComponent& transform)
		{
			if (!light.light)
			{
				light.light = DirectionalLight::Create();
			}
			light.light->SetDirection(transform.GetTransfrom().Forward());
			light.light->SetLightColor(Vector4(light.Color.x, light.Color.y, light.Color.z, light.Intensity));
		});

	m_Manager.Execute<NativeScriptComponent>([&](Snowflake::Entity entity, NativeScriptComponent& script)
		{
			if (!script.Instance)
			{
				script.InstantiateFunc();
				script.Instance->m_Entity = { entity, this };
				script.OnCreateFunc(script.Instance);
			}
			script.OnUpdateFunc(script.Instance);
		});

	m_Manager.Execute<CameraComponent, TransformComponent>([](Snowflake::Entity entity, CameraComponent& camera, TransformComponent& transform)
		{
			if (!camera.camera)
			{
				camera.camera = SceneCamera::Create();
			}
			camera.camera->SetPosition(transform.position);
			camera.camera->SetRotation(transform.rotation);
		});
}

void Alpine::Scene::OnRender()
{
	m_Manager.Execute<CameraComponent>([](Snowflake::Entity entity, CameraComponent& camera)
		{
			if(camera.camera) Renderer::SubmitCamera(camera.camera);
		});
	m_Manager.Execute<MeshComponent>([](Snowflake::Entity entity, MeshComponent& mesh)
		{
			mesh.model->Draw();
		});
	m_Manager.Execute<PointLightComponent>([](Snowflake::Entity entity, PointLightComponent& light)
		{
			Renderer::AddPointLight(*light.light);
		});
	m_Manager.Execute<DirectionalLightComponent>([](Snowflake::Entity entity, DirectionalLightComponent& light)
		{
			Renderer::SubmitDirLight(*light.light);
		});
}

Alpine::Entity Alpine::Scene::CreateEntity()
{
	auto CreatedEntity = m_Manager.CreateEntity();
	Entity entity = { CreatedEntity, this };
	m_SceneEntities[CreatedEntity] = &entity;
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>().Tag = "Entity";
	return entity;
}

void Alpine::Scene::DestroyEntity(Entity entity)
{
	m_Manager.DestroyEntity(entity.m_EntityId);

	m_SceneEntities[entity.m_EntityId] = nullptr;
}

Alpine::Scene::~Scene()
{
	std::map<Snowflake::Entity, Entity*>::iterator it;
	for (it = m_SceneEntities.begin(); it != m_SceneEntities.end(); ++it)
	{
		m_Manager.DestroyEntity(it->first);
	}
}
