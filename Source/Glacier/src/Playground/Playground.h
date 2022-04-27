#pragma once
#include "Engine/Alpine.h"

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
		ImGuiLayer m_ImGuiLayer;
		Alpine::Ref<Alpine::Material> m_Metal;
		PerspectiveCamera m_Camera;
		Alpine::Ref<Alpine::Model> m_Model;
	};
}