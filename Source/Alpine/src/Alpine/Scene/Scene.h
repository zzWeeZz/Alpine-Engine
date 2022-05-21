#pragma once
#include <map>

#include "Alpine/Snowflake/Snowflake.hpp"

namespace Alpine
{
	class Entity;
	class Scene
	{
		friend class Entity;
	public:
		Scene();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		
		~Scene();
	private:
		std::map<Snowflake::Entity, Snowflake::Entity> m_SceneEntities;
		Snowflake::Manager& m_Manager;
	};
}