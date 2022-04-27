#include <iostream>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32 
#include <GLFW/glfw3native.h>
#include "Engine/Engine.h"
#include "ToolBox/Input/Input.h"
#include "ToolBox/Utility/Timer.h"

#include "Application/Application.h"
#include "Engine/RenderSystem/Renderer.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Playground/Playground.h"

int main()
{
	
	Input myKeyInput();
	Alpine::Playground playground;
	ToolBox::Utility::Timer myTimer;
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
	myTimer.Update();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		myTimer.Update();
		playground.Update();
		playground.Render();

		Alpine::Renderer::Begin();
		Alpine::Renderer::DrawStash();
		Alpine::Renderer::End();
		playground.ImGuiRender();
		Alpine::DX11::Present(false);
		
	}
	glfwTerminate();
	return 0;
}