#include "Scene.h"
#include "Entity.h"
#include "DefaultComponents.h"

Alpine::Scene::Scene() : m_Manager(Snowflake::GetManager())
{
}

void Alpine::Scene::Start()
{
	m_Manager.Execute<MeshComponent, TransformComponent>([](Snowflake::Entity entity, MeshComponent& mesh, TransformComponent& transform)
		{
			mesh.model = Model::Create(mesh.MeshPath.string());
			mesh.model->SetPosition(transform.position);
			mesh.model->SetRotation(transform.rotation);
			mesh.model->SetScale(transform.size);
		});
}

void Alpine::Scene::OnUpdate()
{
	m_Manager.Execute<MeshComponent, TransformComponent>([](Snowflake::Entity entity, MeshComponent& mesh, TransformComponent& transform)
		{
			mesh.model->SetPosition(transform.position);
			mesh.model->SetRotation(transform.rotation);
			mesh.model->SetScale(transform.size);
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
