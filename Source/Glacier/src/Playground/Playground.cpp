#include "Playground.h"
#include <imgui/imgui.h>

void Alpine::Playground::Init()
{
	m_Camera.Init({0, 0, 0});
	Renderer::SubmitCamera(std::make_shared<PerspectiveCamera>(m_Camera));
	m_Metal = Material::Create("Metalic");
	m_Metal->AddTexture(Texture::Create("Textures/worn-shiny-metal-ue/worn-shiny-metal-albedo.png"));
	m_Metal->AddTexture(Texture::Create("Textures/worn-shiny-metal-ue/worn-shiny-metal-Roughness.png"));
	m_Metal->AddTexture(Texture::Create("Textures/worn-shiny-metal-ue/worn-shiny-metal-Normal-dx.png"));
	m_Metal->AddTexture(Texture::Create("Textures/mesh-covered-metal1-ao.png"));
	m_Metal->AddTexture(Texture::Create("Textures/worn-shiny-metal-ue/worn-shiny-metal-Metallic.png"));
	m_Metal->AddTexture(Texture::Create("Textures/mesh-covered-metal1-height.png"));
	m_Model = Alpine::Model::Create("Model/M_MED_Gumshoe_Export.fbx", m_Metal);
	m_Model->SetRotation({ 0, 0, 0 });
	m_Model->SetPosition({ 0,10.f, 0 });
	m_Model->SetScale({ 0.1f, 0.1f, 0.1f });
	m_ImGuiLayer.OnAttach();
}

void Alpine::Playground::Update()
{
	m_Camera.Update(0.0001f);
}

void Alpine::Playground::Render()
{
	m_Model->Draw();
}

void Alpine::Playground::ImGuiRender()
{
	m_ImGuiLayer.Begin();
	DX11::Bind();
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
	ImGui::Image(Renderer::GetFrameBuffer()->GetColorAttachment(), {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y});
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::Begin("Inspector");
	/*if (ImGui::CollapsingHeader("Transfrom"))
	{
		static float heliPos[3] = { m_MetalMan.GetPosition().x, m_MetalMan.GetPosition().y, m_MetalMan.GetPosition().z };
		ImGui::DragFloat3("Position", heliPos);
		m_MetalMan.SetPosition({ heliPos[0], heliPos[1], heliPos[2] });

		static float rot[3] = { m_MetalMan.GetRotation().x, m_MetalMan.GetRotation().y, m_MetalMan.GetRotation().z };
		ImGui::DragFloat3("Rotation", rot);

		m_MetalMan.SetRotation({ fmodf(rot[0], 360), fmodf(rot[1], 360), fmodf(rot[2], 360) });
	}*/
	ImGui::End();
	ImGui::ShowDemoWindow();
	ImGui::Begin("Objects");
	ImGui::End();

	ImGui::End();
	m_ImGuiLayer.End();
}
