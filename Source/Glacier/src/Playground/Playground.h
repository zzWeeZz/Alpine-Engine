#pragma once
#include "Alpine/Alpine.h"

namespace Alpine
{

	class Playground
	{
	public:
		void Init();
		void Update(float delta);
		void Render();
		void ImGuiRender();

	private:
		ImGuiLayer m_ImGuiLayer;
		Alpine::Ref<Alpine::Material> m_Metal;
		PerspectiveCamera m_Camera;
		Alpine::Ref<Alpine::Model> m_Model;
		Alpine::Ref<Alpine::Model> m_Ground;
		Ref<DirectionalLight> m_DirectonalLight;
		Ref<PointLight> m_PointLight;
		Vector3 m_LightPos;
	};
}