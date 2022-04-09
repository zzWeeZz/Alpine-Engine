#include "ImGuiLayer.h"

#include <imgui_internal.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Application/Application.h"
#include "DX11/DX11.h"
void Alpine::ImGuiLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.f;
	}

	ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(Alpine::Application::GetWindow()->GetWindow()), true);
	ImGui_ImplDX11_Init(DX11::GetDevice(), DX11::GetDeviceContext());
}
void Alpine::ImGuiLayer::OnDetach()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
}
void Alpine::ImGuiLayer::Begin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_PassthruCentralNode);
	
}

void Alpine::ImGuiLayer::RenderImGui()
{					 
}

void Alpine::ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	

	io.DisplaySize = ImVec2((float)Application::GetWindow()->GetWidth(), (float)Application::GetWindow()->GetHeight());

	//Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		DX11::GetDeviceContext()->OMSetRenderTargets(1, DX11::GetAdressOfRenderTargetView(), DX11::GetDepthStencilView());
	}
}
