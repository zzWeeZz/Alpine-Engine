#include <iostream>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32 
#include <GLFW/glfw3native.h>
#include "Engine/Engine.h"
#include "ToolBox/Input/Input.h"
#include "ToolBox/Utility/Timer.h"
int main()
{
	Engine::Engine myEngine;
	Input myKeyInput();
	ToolBox::Utility::Timer myTimer;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	void KeyCallBack(GLFWwindow * window, int key, int scancode, int action, int mods);
	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Z-Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	Input::GetInstance().SetupKeyInputs(window);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	myEngine.InitD3D(glfwGetWin32Window(window), width, height);
	myTimer.Update();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		
		myEngine.RenderFrame();
		myTimer.Update();
		/* Poll for and process events */
		glfwPollEvents();
		myEngine.Update(myTimer.GetDeltaTime());
		
	}

	glfwTerminate();
	myEngine.CleanD3D();
	return 0;
}