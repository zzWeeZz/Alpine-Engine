#include "EditorLayer.h"

#include "ImGuizmo.h"
#include "ToolBox/Utility/Chrono.h"

void Alpine::EditorLayer::OnAttach()
{
	m_ImGuiLayer.OnAttach();
	m_ActiveScene = std::make_shared<Scene>();

	/*{
		auto entity = m_ActiveScene->CreateEntity();
		entity.GetComponent<TagComponent>().Tag = "Camera";

		entity.AddComponent<TransformComponent>().position = Vector3{ 0, 0, 0 };
		entity.AddComponent<CameraComponent>();
	}*/
	{
		auto entity = m_ActiveScene->CreateEntity();
		entity.AddComponent<MeshComponent>().MeshPath = "Model/Sponza/SponzaForTGE.fbx";
		entity.GetComponent<TagComponent>().Tag = "Sponza";

		entity.AddComponent<TransformComponent>().position = Vector3{ 0, 0, 0 };
	}
	{
		auto entity = m_ActiveScene->CreateEntity();
		entity.AddComponent<PointLightComponent>();
		entity.GetComponent<TagComponent>().Tag = "point light";
		entity.GetComponent<TransformComponent>();
	}
	{
		auto entity = m_ActiveScene->CreateEntity();
		entity.AddComponent<DirectionalLightComponent>();
		entity.GetComponent<TagComponent>().Tag = "Direction light";
		entity.GetComponent<TransformComponent>();
	}

	m_ActiveScene->Start();
	m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>(m_ActiveScene);
	m_ViewportPanel = CreateRef<ViewportPanel>();
	m_EditorCamera = EditorCamera::Create({ 0, 0, 0 });
}

void Alpine::EditorLayer::OnDetach()
{
	m_ImGuiLayer.OnDetach();
}

void Alpine::EditorLayer::Begin()
{
	m_EditorCamera->Update(ToolBox::Chrono::Timestep());
	m_ActiveScene->OnUpdate();
}

void Alpine::EditorLayer::RenderImGui()
{
	m_ActiveScene->OnRender();
	Renderer::SubmitEditorCamera(m_EditorCamera);
	m_ImGuiLayer.Begin();
	m_ImGuiLayer.RenderImGui();
	static bool pOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpacePanel Demo", &pOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpacePanel
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false))
				pOpen = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	m_SceneHierarchyPanel->OnImGuiRender();
	m_ViewportPanel->OnImGuiRender(m_EditorCamera, m_SceneHierarchyPanel);
	
	

	ImGui::End();
	
	m_ImGuiLayer.End();

}

void Alpine::EditorLayer::End()
{
}
