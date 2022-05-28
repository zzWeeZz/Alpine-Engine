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
		Entity() = default;
		Entity(Snowflake::Entity entity, Scene* scene);

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Manager.HasComponent<T>(m_EntityId);
		}

		template<typename T>
		T& AddComponent()
		{
			return m_Scene->m_Manager.AddComponent<T>(m_EntityId);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Manager.RemoveComponent<T>(m_EntityId);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Manager.GetComponent<T>(m_EntityId);
		}

		bool operator==(const Entity& other) const
		{
			return m_EntityId == other.m_EntityId && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !operator==(other);
		}

		bool operator()()
		{
			return m_EntityId != Snowflake::InvalidEntity;
		}

		uint32_t GetId() const
		{
			return m_EntityId;
		}

	private:
		Snowflake::Entity m_EntityId{ Snowflake::InvalidEntity };
		Scene* m_Scene = nullptr;
	};
}