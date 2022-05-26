#pragma once
#include "Alpine/Alpine.h"
#include "Glacier/Panels/SceneHierarchyPanel.h"

namespace Alpine
{

	class Playground
	{
	public:
		void Init();
		void Update();
		void Render();
		void ImGuiRender();

	private:
		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		ImGuiLayer m_ImGuiLayer;
	};
}