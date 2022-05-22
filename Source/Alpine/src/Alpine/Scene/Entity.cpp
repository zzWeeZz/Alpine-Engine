#include "Entity.h"

Alpine::Entity::Entity(Snowflake::Entity entity, Ptr<Scene> scene)
	: m_EntityId(entity), m_Scene(scene)
{
}
