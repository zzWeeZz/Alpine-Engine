#pragma once
#include <map>
#include "Alpine/Snowflake/Snowflake.hpp"

namespace Alpine
{
	class Entity;

	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
	public:
		Scene();

		void Start();
		void OnUpdate();
		void OnRender();

		void Serialize(const std::string& location);
		void Deserialize(const std::string& location);

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		
		~Scene();
	private:
		std::map<Snowflake::Entity, Entity*> m_SceneEntities;
		Snowflake::Manager& m_Manager;
	};
}