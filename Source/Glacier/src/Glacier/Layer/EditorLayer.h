#pragma once
#include "imgui/imgui.h"
#include "Alpine/Alpine.h"
#include "Glacier/Panels/SceneHierarchyPanel.h"
#include "Glacier/Panels/ViewportPanel.h"

namespace Alpine
{
	class EditorLayer
	{
	public:
		void OnAttach();
		void OnDetach();
		void Begin();
		void RenderImGui();
		void ProccessPath(const std::filesystem::path& path);
		void End();
	private:
		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Ref<ViewportPanel> m_ViewportPanel;
		Ref<EditorCamera> m_EditorCamera;
		ImGuiLayer m_ImGuiLayer;
		Ref<Scene> m_ActiveScene;
	};
}
