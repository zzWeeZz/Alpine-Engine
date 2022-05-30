#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include "ImGuizmo.h"

Alpine::SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
{
	SetContext(context);
}

void Alpine::SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
{
	m_Context = context;
	m_SelectedEntity = {};
}

void Alpine::SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Hierarchy");

	m_Context->m_Manager.ForEach([&](Snowflake::Entity ent)
		{
			Entity entity = { ent, m_Context.get() };
			DrawEntityNode(entity);

		});
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_SelectedEntity = {};
	}
	bool destroy = false;
	if (ImGui::BeginPopupContextWindow("one", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			m_Context->CreateEntity();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
	if (m_SelectedEntity()) DrawImGizmo(m_SelectedEntity);

	ImGui::Begin("Properties");
	if (m_SelectedEntity())
	{
		DrawComponents(m_SelectedEntity);
	}
	ImGui::End();
}

void Alpine::SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	auto& tag = entity.GetComponent<TagComponent>();
	ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetId(), flags, tag.Tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectedEntity = entity;
	}

	bool deleted = false;
	if (ImGui::BeginPopupContextWindow(std::to_string(entity.GetId()).c_str(), ImGuiPopupFlags_MouseButtonRight))
	{
		if (ImGui::MenuItem("destroy Entity"))
		{
			deleted = true;
		}
		ImGui::EndPopup();
	}
	if (isOpened)
	{
		ImGui::TreePop();
	}
	if (deleted)
	{
		m_Context->DestroyEntity(m_SelectedEntity);
		if (m_SelectedEntity == entity)
		{
			m_SelectedEntity = {};
		}
	}
}

void Alpine::SceneHierarchyPanel::DrawComponents(Entity entity)
{
	if (entity.HasComponent<TagComponent>())
	{
		auto& tag = entity.GetComponent<TagComponent>();
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, tag.Tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			tag.Tag = buffer;
		}
	}


	const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

	if (entity.HasComponent<TransformComponent>())
	{
		auto& tf = entity.GetComponent<TransformComponent>();
		bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform Component");
		ImGui::SameLine();
		if(ImGui::Button("..."))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
			{
				removeComponent = true;
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			float imPos[3] = { 0 };
			memcpy(imPos, &tf.position, sizeof(Vector3));
			if (ImGui::DragFloat3("Position", imPos, 0.1f))
			{
				memcpy(&tf.position, imPos, sizeof(Vector3));
			}
			float imRot[3] = { 0 };
			memcpy(imRot, &tf.rotation, sizeof(Vector3));
			if (ImGui::DragFloat3("Rotation", imRot, 0.1f))
			{
				memcpy(&tf.rotation, imRot, sizeof(Vector3));
			}
			float imScale[3] = { 0 };
			memcpy(imScale, &tf.size, sizeof(Vector3));
			if (ImGui::DragFloat3("Scale", imScale, 0.1f))
			{
				memcpy(&tf.size, imScale, sizeof(Vector3));
			}
			ImGui::TreePop();
		}
		if (removeComponent)
		{
			entity.RemoveComponent<TransformComponent>();
		}
	}
	if (entity.HasComponent<PointLightComponent>())
	{
		auto& plc = entity.GetComponent<PointLightComponent>();
		if (ImGui::TreeNodeEx((void*)typeid(PointLightComponent).hash_code(), flags, "Pointlight Component"))
		{
			float col[3] = { 0 };
			memcpy(col, &plc.Color, sizeof(Vector3));
			if (ImGui::ColorEdit3("Color", col))
			{
				memcpy(&plc.Color, col, sizeof(Vector3));
			}

			if (ImGui::DragFloat("Intensity", &plc.Intensity, 0.1f))
			{
				plc.Intensity = std::max(0.0f, plc.Intensity);
			}

			if (ImGui::DragFloat("Radius", &plc.Range, 0.1f))
			{
				plc.Range = std::max(0.0f, plc.Range);
			}
			if (ImGui::SliderFloat("Falloff", &plc.Falloff, 0.0f, 1.0f))
			{
				plc.Falloff = std::max(0.0f, plc.Falloff);
			}
			ImGui::TreePop();
		}
	}
	if (entity.HasComponent<DirectionalLightComponent>())
	{
		auto& plc = entity.GetComponent<DirectionalLightComponent>();
		if (ImGui::TreeNodeEx((void*)typeid(DirectionalLightComponent).hash_code(), flags, "Directionlight Component"))
		{
			float col[3] = { 0 };
			memcpy(col, &plc.Color, sizeof(Vector3));
			if (ImGui::ColorEdit3("Color", col))
			{
				memcpy(&plc.Color, col, sizeof(Vector3));
			}

			if (ImGui::DragFloat("Intensity", &plc.Intensity, 0.1f))
			{
				plc.Intensity = std::max(0.0f, plc.Intensity);
			}
			ImGui::TreePop();
		}
	}
}

void Alpine::SceneHierarchyPanel::DrawImGizmo(Entity entity)
{


}
