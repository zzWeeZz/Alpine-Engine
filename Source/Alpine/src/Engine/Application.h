#pragma once
#include <GLFW/glfw3.h>
#include <utility>
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32 

struct Window
{
	GLFWwindow* window;
	int screenHeight;
	int screenWidth;
};

class Application
{
public:
	// creates new instance of the window
	static bool CreateNewWindow(int width, int height);
	static GLFWwindow* GetWindow();
	static float GetAspectRatio();
	static std::pair<int, int> GetWindowSize();
private:
	static void ResizeCallBack(GLFWwindow* window, int width, int height);
	static Window myWindow;
};