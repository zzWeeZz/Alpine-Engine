#pragma once
#include "Entity.h"
namespace Alpine
{
	class ScriptableEntity
	{
		friend class Scene;
	public:
		template<typename T>
		bool HasComponent()
		{
			return m_Entity.HasComponent<T>();
		}

		template<typename T>
		T& AddComponent()
		{
			return m_Entity.AddComponent<T>();
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Entity.RemoveComponent<T>();
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	private:
		Entity m_Entity;
	};
}
