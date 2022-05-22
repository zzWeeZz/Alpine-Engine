#include "Scene.h"
#include "Entity.h"
#include "DefaultComponents.h"

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
			if(!mesh.model)
			{
				mesh.model = Model::Create(mesh.MeshPath.string());
			}
			mesh.model->SetPosition(transform.position);
			mesh.model->SetRotation(transform.rotation);
			mesh.model->SetScale(transform.size);
		});

	m_Manager.Execute<NativeScriptComponent>([&](Snowflake::Entity entity, NativeScriptComponent& script)
		{
			if(!script.Instance)
			{
				script.InstantiateFunc();
				script.Instance->m_Entity = { entity, this };
				script.OnCreateFunc(script.Instance);
			}
			script.OnUpdateFunc(script.Instance);
		});
}

void Alpine::Scene::OnRender()
{
	m_Manager.Execute<MeshComponent>([](Snowflake::Entity entity, MeshComponent& mesh)
		{
			mesh.model->Draw();
		});
}

Alpine::Entity Alpine::Scene::CreateEntity()
{
	auto CreatedEntity = m_Manager.CreateEntity();
	m_SceneEntities[CreatedEntity] = CreatedEntity;
	Entity entity = { CreatedEntity, this };
	entity.AddComponent<TransformComponent>();
	return entity;
}

void Alpine::Scene::DestroyEntity(Entity entity)
{
	m_Manager.DestroyEntity(entity.m_EntityId);

	m_SceneEntities[entity.m_EntityId] = Snowflake::InvalidEntity;
}

Alpine::Scene::~Scene()
{
	std::map<Snowflake::Entity, Snowflake::Entity>::iterator it;
	for (it = m_SceneEntities.begin(); it != m_SceneEntities.end(); ++it)
	{
		m_Manager.DestroyEntity(it->second);
	}
}
