#include <iostream>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32 
#include <GLFW/glfw3native.h>

#include "ToolBox/Input/Input.h"
#include "ToolBox/Utility/Timer.h"

#include "Application/Application.h"
#include "Alpine/RenderSystem/Renderer.h"
#include "Alpine/ImGui/ImGuiLayer.h"
#include "Glacier/Layer/EditorLayer.h"
#include "ToolBox/Utility/Chrono.h"
Alpine::EditorLayer editorLayer;
void DropCallback(GLFWwindow* window, int count, const char* paths[])
{
	for (int i = 0; i < count; i++)
	{
		editorLayer.ProccessPath(paths[i]);
	}
}


int main()
{
	Input myKeyInput();
	if (!glfwInit())
		return -1;
	if (!Alpine::Application::CreateNewWindow("Alpine", 1280, 720))
	{
		return -1;
	}
	auto window = static_cast<GLFWwindow*>(Alpine::Application::GetWindow()->GetWindow());
	Input::GetInstance().SetupKeyInputs(window);
	Alpine::DX11::Initialize(1280, 720, false);
	Alpine::Renderer::Initalize();
	glfwMakeContextCurrent(window);

	glfwSetDropCallback(window, DropCallback);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	editorLayer.OnAttach();
	ToolBox::Chrono::UpdateTimeData();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ToolBox::Chrono::UpdateTimeData();
		editorLayer.Begin();
		Alpine::DX11::ClearView();
		Alpine::DX11::Bind();
		editorLayer.RenderImGui();
		Alpine::Renderer::Begin();
		Alpine::DX11::Present(true);
		
	}
	glfwTerminate();
	return 0;
}
