#include "ViewportPanel.h"

#include <imgui.h>

#include "ImGuizmo.h"
#include "Alpine/RenderSystem/Renderer.h"
#include "Alpine/Scene/DefaultComponents.h"
#include "Application/Application.h"
#include "Glacier/Panels/SceneHierarchyPanel.h"
#include "ToolBox/Input/Input.h"

Alpine::ViewportPanel::ViewportPanel()
{
	m_FrameBuffer = NULL;
}

void Alpine::ViewportPanel::OnImGuiRender(Ref<EditorCamera> camera, Ref<SceneHierarchyPanel> sceneHierarchy)
{
	m_FrameBuffer = Renderer::GetFrameBuffer().get();
	memcpy(&*m_FrameBuffer, &*Renderer::GetFrameBuffer().get(), sizeof(FrameBuffer));
	static bool open = true;
	static int mode = 0;
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ViewPort", &open, ImGuiWindowFlags_NoCollapse);
	ImGui::Combo("Mode", &mode, "Translate\0Rotate\0Scale\0");
	if (m_FrameBuffer->GetSpecification().width != ImGui::GetWindowWidth() || m_FrameBuffer->GetSpecification().height != ImGui::GetWindowHeight())
	{
		m_FrameBuffer->Resize(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
		camera->SetAspectRatio((float)m_FrameBuffer->GetSpecification().width / (float)m_FrameBuffer->GetSpecification().height);
	}
	ImGui::Image(m_FrameBuffer->GetColorAttachment(), { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
	

	if (Input::GetInstance().GetKeyDown(Keys::W)) mode = 0;
	if (Input::GetInstance().GetKeyDown(Keys::E)) mode = 1;
	if (Input::GetInstance().GetKeyDown(Keys::R)) mode = 2;

	if (sceneHierarchy->GetSelectedEntity()())
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		float view[16] = { 0 };
		memcpy(view, camera->GetViewMatrix().m, sizeof(float) * 16);
		float proj[16] = { 0 };
		memcpy(proj, camera->GetProjectionMatrix().m, sizeof(float) * 16);
		auto entity = sceneHierarchy->GetSelectedEntity();
		auto& tf = entity.GetComponent<TransformComponent>();
		auto matrix = tf.GetTransfrom();
		float trans[16] = { 0 };
		memcpy(trans, matrix.m, sizeof(float) * 16);
		if(mode == 0)
		{
			ImGuizmo::Manipulate(&view[0], &proj[0], ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, &trans[0]);
		}
		else if(mode == 1)
		{
			ImGuizmo::Manipulate(&view[0], &proj[0], ImGuizmo::ROTATE, ImGuizmo::LOCAL, &trans[0]);
		}
		else if (mode == 2)
		{
			ImGuizmo::Manipulate(&view[0], &proj[0], ImGuizmo::SCALE, ImGuizmo::LOCAL, &trans[0]);
		}

		if(ImGuizmo::IsUsing())
		{
			Matrix mat;
			memcpy(mat.m, &trans[0], sizeof(float) * 16);
			Vector3 scale;
			Quaternion quaternion;
			Vector3 position;
			mat.Decompose(scale, quaternion, position);
			tf.position = position;
			tf.size = scale;
			Vector3 forward = Vector3(quaternion.ToEuler().y, quaternion.ToEuler().x, quaternion.ToEuler().z);
			tf.rotation = forward;
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}