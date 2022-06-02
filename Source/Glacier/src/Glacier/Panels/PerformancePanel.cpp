#include "PerformancePanel.h"
#include "Alpine/RenderSystem/Renderer.h"
#include "imgui/imgui.h"
void Alpine::PerformancePanel::OnRenderImGui()
{
	ImGui::Begin("Performance");
	ImGui::Text("Renderer");
	ImGui::Text("Draw calls: %d.", Renderer::GetDrawCallCount());
	ImGui::Text("Response time %.3f", ImGui::GetIO().DeltaTime);
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::End();
}

Alpine::Ref<Alpine::PerformancePanel> Alpine::PerformancePanel::Create()
{
	return CreateRef<PerformancePanel>();
}
