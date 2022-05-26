#pragma once
#include "Alpine/Alpine.h"

namespace Alpine
{
	class SceneHierarchyPanel
	{
		friend class Scene;
	public:
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		Entity GetSelectedEntity() const { return m_SelectedEntity; }
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		void DrawImGizmo(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
	};
}