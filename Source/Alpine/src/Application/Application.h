#pragma once
#include <GLFW/glfw3.h>
#include <DirectXTK/SimpleMath.h>
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
	Application();
	// creates new instance of the window
	static bool CreateNewWindow(int width, int height);
	static GLFWwindow* GetWindow();
	static float GetAspectRatio();

	static DirectX::SimpleMath::Vector2 GetWindowSize();
private:
	static Application myInstance;
	static void ResizeCallBack(GLFWwindow* window, int width, int height);
	Window myWindow;
};