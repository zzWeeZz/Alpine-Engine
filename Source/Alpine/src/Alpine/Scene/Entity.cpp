#include "Entity.h"

Alpine::Entity::Entity(Snowflake::Entity entity, Scene* scene)
	: m_EntityId(entity), m_Scene(scene)
{
}
