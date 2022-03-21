#include <iostream>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32 
#include <GLFW/glfw3native.h>
#include "Engine/Engine.h"
int main()
{
	Engine::Engine myEngine;
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Z-Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	myEngine.InitD3D(glfwGetWin32Window(window), 1920, 1080);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		myEngine.Update();
		myEngine.RenderFrame();

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	myEngine.CleanD3D();
	return 0;
}