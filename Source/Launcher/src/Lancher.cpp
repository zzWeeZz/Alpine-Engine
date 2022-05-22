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
#include "Playground/Playground.h"
#include "ToolBox/Utility/Chrono.h"


int main()
{
	Input myKeyInput();
	Alpine::Playground playground;
	if (!glfwInit())
		return -1;
	if (!Alpine::Application::CreateNewWindow("Alpine", 1280, 720))
	{
		return -1;
	}
	auto window = static_cast<GLFWwindow*>(Alpine::Application::GetWindow()->GetWindow());
	Alpine::ImGuiLayer imguiLayer;
	Input::GetInstance().SetupKeyInputs(window);
	Alpine::DX11::Initialize(1280, 720, false);
	Alpine::Renderer::Initalize();
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	playground.Init();
	ToolBox::Chrono::UpdateTimeData();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ToolBox::Chrono::UpdateTimeData();
		playground.Update();

		Alpine::Renderer::Begin();
		Alpine::DX11::ClearView();
		Alpine::Renderer::DrawStash();
		Alpine::DX11::Bind();
		playground.Render();
		Alpine::Renderer::End();
		playground.ImGuiRender();
		Alpine::DX11::Present(true);
		
	}
	glfwTerminate();
	return 0;
}