#pragma once
#include <filesystem>
#include <map>
#include "Alpine/Snowflake/Snowflake.hpp"

namespace Alpine
{
	class Entity;



	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	public:
		Scene();

		void Start();
		void OnUpdate();
		void OnRender();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		
		~Scene();
	private:
		std::map<Snowflake::Entity, Entity*> m_SceneEntities;
		Snowflake::Manager& m_Manager;
	};
}