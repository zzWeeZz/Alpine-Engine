#include <iostream>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32 
#include <GLFW/glfw3native.h>
#include "Engine/Engine.h"
#include "ToolBox/Input/KeyInput.h"
int main()
{
	Engine::Engine myEngine;
	KeyInput myKeyInput();
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

	KeyInput::GetInstance().SetupKeyInputs(window);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	myEngine.InitD3D(glfwGetWin32Window(window), width, height);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		
		myEngine.RenderFrame();
	
		/* Poll for and process events */
		glfwPollEvents();
		myEngine.Update();
		
	}

	glfwTerminate();
	myEngine.CleanD3D();
	return 0;
}