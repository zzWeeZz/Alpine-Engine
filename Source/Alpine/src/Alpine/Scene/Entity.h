#pragma once
#include "Scene.h"
#include "Alpine/DX11/Utilities.h"
#include "Alpine/Snowflake/Snowflake.hpp"

namespace Alpine
{
	class Entity
	{
		friend class Scene;
	public:
		Entity(Snowflake::Entity entity, Ptr<Scene> scene);

		template<typename T>
		bool HasComponent()
		{
			return m_Scene.m_Manager.HasComponent<T>(m_EntityId);
		}

		template<typename T>
		T& AddComponent()
		{
			return m_Scene.m_Manager.AddComponent<T>(m_EntityId);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene.m_Manager.RemoveComponent<T>(m_EntityId);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene.m_Manager.GetComponent<T>(m_EntityId);
		}

	private:
		Snowflake::Entity m_EntityId;
		Ptr<Scene> m_Scene;
	};
}