#include "EditorLayer.h"

#include "ImGuizmo.h"
#include "Alpine/Scene/SceneSerializer.h"
#include "ToolBox/Utility/Chrono.h"
#include "Glacier/Utilities/Utilities.h"
#include "Alpine/Utils/PlatformUtils.h"

void Alpine::EditorLayer::OnAttach()
{
	m_ImGuiLayer.OnAttach();
	m_ActiveScene = std::make_shared<Scene>();
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
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				m_ActiveScene = CreateRef<Scene>();
				m_SceneHierarchyPanel->SetContext(m_ActiveScene);
			}
		
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				std::string path = FileDialogs::OpenFile("Alpine Scene (*.frost)\0*.frost\0");
				if (!path.empty())
				{
					m_ActiveScene = CreateRef<Scene>();
					m_SceneHierarchyPanel->SetContext(m_ActiveScene);
					SceneSerializer serializer(m_ActiveScene);
					serializer.Deserialize(path);
				}
			}

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				std::string path = FileDialogs::SaveFile("Alpine Scene (*.frost)\0*.frost\0");
				if (!path.empty())
				{
					SceneSerializer serializer(m_ActiveScene);
					serializer.Serialize(path);
				}

			}

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

void Alpine::EditorLayer::ProccessPath(const std::filesystem::path& path)
{
	if (Glacier::GetFileType(path) == Glacier::FBX)
	{
		{
			auto entity = m_ActiveScene->CreateEntity();
			entity.AddComponent<MeshComponent>().MeshPath = path;
			entity.GetComponent<TagComponent>().Tag = path.filename().string();
		}
	}
	else if(Glacier::GetFileType(path) == Glacier::Scene)
	{
		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel->SetContext(m_ActiveScene);
		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(path.string());
	}
}

void Alpine::EditorLayer::End()
{
}
