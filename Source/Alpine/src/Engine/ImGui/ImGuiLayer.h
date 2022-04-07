#pragma once

namespace Alpine
{
	class ImGuiLayer
	{
	public:
		void OnAttach();
		void OnDetach();
		void Begin();
		void RenderImGui();
		void End();

	};
}