#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>
#include <array>
#include <bitset>
#include <cassert>
#include <unordered_map>
#include <functional>

namespace Snowflake
{

	constexpr uint32_t InvalidEntity = ~0;

	using Entity = uint32_t;

	template<class Component>
	class ComponentPool;

	
	class IPool
	{
	public:
		virtual void RegisterEntity(Entity entity) = 0;
		virtual void DeRegisterEntity(Entity entity) = 0;
		virtual bool IsEntityRegistered(Entity entity) = 0;
	};

	class Manager
	{
	public:
		Manager()
		{
			s_Instance = *this;
		}

		Entity CreateEntity()
		{
			Entity entity = m_Entities.size();
			m_Entities.emplace_back(entity);
			return entity;
		}

		// fix complexety to O(1)
		bool DestroyEntity(Entity& entity)
		{
			if (entity == InvalidEntity) return false;
			auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
			if (it != m_Entities.end())
			{
				if (m_Entities.size() > 1)
					std::swap(*it, m_Entities.back());
				m_Entities.pop_back();
				while (!m_Registry[entity].empty())
				{
					static_cast<IPool*>(m_ComponentPools[m_Registry[entity].back()])->DeRegisterEntity(entity);
					m_Registry[entity].pop_back();
				}
				entity = InvalidEntity;
				return true;
			}
			return false;
		}

		bool ValidateEntity(Entity entity)
		{
			auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
			return it != m_Entities.end();
		}

		// TODO: Create pool if it does not exist.
		template<class TComponent>
		TComponent& AddComponent(Entity entity)
		{
			if (entity == InvalidEntity)
			{
				throw std::invalid_argument("AddComponent called with invalid entity.");
			}
			auto& pool = MakeOrGetPool<TComponent>();

			pool.RegisterEntity(entity);
			m_Registry[entity].push_back(typeid(TComponent).hash_code());
			return pool.GetComponent(entity);
		}

		template<class TComponent>
		TComponent* TryGetComponent(Entity entity)
		{
			if (entity == InvalidEntity)
			{
				throw std::invalid_argument("TryGetComponent called with invalid entity.");
			}
			auto& component = MakeOrGetPool<TComponent>();
			if (HasComponent<TComponent>(entity))
			{
				return &component.GetComponent(entity);
			}
			return nullptr;
		}

		template<class TComponent>
		TComponent& GetComponent(Entity entity)
		{
			if (entity == InvalidEntity)
			{
				throw std::invalid_argument("GetComponent called with invalid entity.");
			}
			auto& component = MakeOrGetPool<TComponent>();
			return component.GetComponent(entity);
		}

		template<class TComponent>
		void RemoveComponent(Entity entity)
		{
			if (entity == InvalidEntity)
			{
				throw std::invalid_argument("RemoveComponent called with invalid entity.");
			}
			auto& pool = MakeOrGetPool<TComponent>();
			pool.DeRegisterEntity(entity);
			auto& registry = m_Registry[entity];
			auto it = std::find(registry.begin(), registry.end(), typeid(TComponent).hash_code());
			if (it != registry.end())
			{
				registry.erase(it);
			}
		}

		template<typename TComponent>
		bool HasComponent(Entity entity)
		{
			if (entity == InvalidEntity)
			{
				throw std::invalid_argument("HasComponent called with invalid entity.");
			}
			auto& component = MakeOrGetPool<TComponent>();
			return component.IsEntityRegistered(entity);
		}

		template<typename ...TComponents>
		bool HasComponents(Entity entity)
		{
			if (entity == InvalidEntity)
			{
				throw std::invalid_argument("HasComponents called with invalid entity.");
			}
			return (HasComponent<TComponents>(entity) && ...);
		}
		template<class ...TComponents, class TFunction>
		void Execute(TFunction&& func)
		{
			for (auto entity : m_Entities)
			{
				if (HasComponents<TComponents...>(entity))
				{
					func(entity, GetComponent<TComponents>(entity)...);
				}
			}
		}
		static Manager& GetManager()
		{
			return s_Instance;
		}
	private:

		template<class TComponent>
		ComponentPool<TComponent>& MakeOrGetPool()
		{
			size_t hash = typeid(TComponent).hash_code();
			auto it = m_ComponentPools.find(hash);
			if (it == m_ComponentPools.end())
			{
				m_ComponentPools[hash] = new ComponentPool<TComponent>();
			}
			return *static_cast<ComponentPool<TComponent>*>(m_ComponentPools[hash]);
		}
		template<class TComponent>
		ComponentPool<TComponent>& GetPoolWithHash(size_t hash)
		{
			return *static_cast<ComponentPool*>(m_ComponentPools[hash]);
		}

		static Snowflake::Manager s_Instance;
		std::vector<Entity> m_Entities;
		std::unordered_map<Entity, std::vector<size_t>> m_Registry;
		std::unordered_map<size_t, void*> m_ComponentPools;

	};
	Snowflake::Manager Manager::s_Instance;

	inline Manager& GetManager()
	{
		return Manager::GetManager();
	}

	

	template<class Component>
	class ComponentPool : public IPool
	{
		friend Manager;
	public:

		void RegisterEntity(Entity entity) override
		{
			if (!m_RegisteredEntities[entity])
			{
				m_Components[entity] = Component();
				m_RegisteredEntities[entity] = true;
			}
		}

		void DeRegisterEntity(Entity entity) override
		{
			m_RegisteredEntities[entity] = false;
			m_Components[entity] = Component();
		}

		bool IsEntityRegistered(Entity entity)override
		{
			return m_RegisteredEntities[entity];
		}

		Component& GetComponent(Entity entity)
		{
			return m_Components[entity];
		}
	private:
		std::array<Component, 8192> m_Components = {};
		std::bitset<8192> m_RegisteredEntities = { false };
	};
}