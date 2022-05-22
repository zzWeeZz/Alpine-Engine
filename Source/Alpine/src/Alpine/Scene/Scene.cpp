#include "Scene.h"
#include "Entity.h"
#include "DefaultComponents.h"

Alpine::Scene::Scene() : m_Manager(Snowflake::GetManager())
{
}

Alpine::Entity Alpine::Scene::CreateEntity()
{
	auto CreatedEntity = m_Manager.CreateEntity();
	m_SceneEntities[CreatedEntity] = CreatedEntity;
	Entity entity = { CreatedEntity, std::enable_shared_from_this(this)};
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
