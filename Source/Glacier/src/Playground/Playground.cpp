#include "Playground.h"
#include <imgui/imgui.h>
#include "Alpine/Scene/DefaultComponents.h"
#include "ToolBox/Utility/Chrono.h"

void Alpine::Playground::Init()
{
	m_Camera.Init({ 0, 0, 0 });

	class TestScript : public ScriptableEntity
	{
	public:
		void OnCreate()
		{
			GetComponent<TransformComponent>().size = Vector3(0.5, 0.5, 0.5f);
		}

		void OnUpdate()
		{
			static float YValue = 0;
			YValue += ToolBox::Chrono::Timestep() * 10;
			if (HasComponent<TransformComponent>())
			{
				auto& tf = GetComponent<TransformComponent>();
			}
		}

		void OnDestroy()
		{

		}
	};

	{
		auto entity = m_Scene.CreateEntity();
		entity.AddComponent<MeshComponent>().MeshPath = "Model/gremlin_sk.fbx";
		entity.AddComponent<NativeScriptComponent>().Bind<TestScript>();
		entity.GetComponent<TagComponent>().Tag = "Gremlin";

		entity.AddComponent<TransformComponent>().position = Vector3{ -50, 0, 0 };
	}
	{
		auto entity = m_Scene.CreateEntity();
		entity.AddComponent<MeshComponent>().MeshPath = "Model/gremlin_sk.fbx";
		entity.AddComponent<NativeScriptComponent>().Bind<TestScript>();
		entity.GetComponent<TagComponent>().Tag = "Gremlin happy";

		entity.GetComponent<TransformComponent>().position = Vector3{ 0, 0, 0 };

	}
	{
		auto entity = m_Scene.CreateEntity();
		entity.AddComponent<MeshComponent>().MeshPath = "Model/gremlin_sk.fbx";
		entity.AddComponent<NativeScriptComponent>().Bind<TestScript>();
		entity.GetComponent<TagComponent>().Tag = "Glenn";
		entity.GetComponent<TransformComponent>().position = Vector3{ 50, 0, 0 };

	}

	Renderer::SubmitCamera(std::make_shared<PerspectiveCamera>(m_Camera));


	m_DirectonalLight = DirectionalLight::Create();
	m_DirectonalLight->SetLightColor({ 1,1,1, 1 });
	m_DirectonalLight->SetDirection({ 1,1, 0 });
	Renderer::SubmitDirLight(*m_DirectonalLight.get());
	m_PointLight = PointLight::Create();
	m_PointLight->SetLightColor({ 1,1,1, 50.f });
	m_PointLight->SetPosition({ 0,5,-10 });
	m_PointLight->SetRange(30);
	m_PointLight->SetFallOff(0.5f);
	m_ImGuiLayer.OnAttach();
	m_Scene.Start();
	m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>(std::make_shared<Scene>(m_Scene));
}

void Alpine::Playground::Update()
{
	m_Camera.Update(ToolBox::Chrono::Timestep());
	static float angle = 0;
	angle += ToolBox::Chrono::Timestep() * 90;
	m_Scene.OnUpdate();
}

void Alpine::Playground::Render()
{
	Renderer::SubmitCamera(std::make_shared<PerspectiveCamera>(m_Camera));
	Renderer::AddPointLight(*m_PointLight.get());
	m_Scene.OnRender();
	//m_Ground->Draw();
}

void Alpine::Playground::ImGuiRender()
{
	m_ImGuiLayer.Begin();
	m_ImGuiLayer.RenderImGui();
	static bool pOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
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

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &pOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ViewPort", &pOpen, ImGuiWindowFlags_NoCollapse);
	if (Renderer::GetFrameBuffer()->GetSpecification().width != ImGui::GetWindowWidth() || Renderer::GetFrameBuffer()->GetSpecification().height != ImGui::GetWindowHeight())
	{
		Renderer::GetFrameBuffer()->Resize(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
		m_Camera.SetAspectRatio(ImGui::GetWindowWidth() / ImGui::GetWindowHeight());
	}
	ImGui::Image(Renderer::GetFrameBuffer()->GetColorAttachment(), { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::Begin("Light");
	ImGui::Text("Light");
	static float light_pos[3] = { 0.0f, 0.0f, 0.0f };
	ImGui::DragFloat3("Position", light_pos, -10.0f, 10.0f);
	m_PointLight->SetPosition({ light_pos[0], light_pos[1], light_pos[2] });
	static float lightFallOff = m_PointLight->GetFallOff();
	ImGui::SliderFloat("FallOff", &lightFallOff, 0.0f, 1.0f);
	m_PointLight->SetFallOff(lightFallOff);
	static float range = m_PointLight->GetRange();
	ImGui::SliderFloat("Range", &range, 0.0f, 100.0f);
	m_PointLight->SetRange(range);
	ImGui::End();
	m_SceneHierarchyPanel->OnImGuiRender();
	ImGui::Begin("Performance");
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::End();
	m_ImGuiLayer.End();
}
