#include <iostream>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32 
#include <GLFW/glfw3native.h>
#include "Engine/Engine.h"
#include "ToolBox/Input/Input.h"
#include "ToolBox/Utility/Timer.h"

#include "Engine/Application.h"
#include "Engine/ImGui/ImGuiLayer.h"

int main()
{
	Alpine::Engine myEngine;
	Input myKeyInput();
	ToolBox::Utility::Timer myTimer;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	void KeyCallBack(GLFWwindow * window, int key, int scancode, int action, int mods);
	/* Create a windowed mode window and its OpenGL context */
	if (!Application::CreateNewWindow(1280, 720))
	{
		return -1;
	}
	auto window = Application::GetWindow();
	Alpine::ImGuiLayer imguiLayer;
	Input::GetInstance().SetupKeyInputs(window);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	myEngine.InitD3D(glfwGetWin32Window(window), width, height);
	myTimer.Update();
	imguiLayer.OnAttach();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		imguiLayer.Begin();
		imguiLayer.RenderImGui();
		imguiLayer.End();
		myTimer.Update();
		myEngine.RenderFrame();
		/* Poll for and process events */
		glfwPollEvents();
		myEngine.Update(myTimer.GetDeltaTime());
		
	}
	imguiLayer.OnDetach();
	glfwTerminate();
	myEngine.CleanD3D();
	return 0;
}