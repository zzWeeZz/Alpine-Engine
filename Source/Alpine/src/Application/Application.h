#pragma once
#include <GLFW/glfw3.h>
#include <DirectXTK/SimpleMath.h>
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32
#include "Window/GlfwWindow.h"
namespace Alpine
{

	class Application
	{
	public:
		Application();
		// creates new instance of the window
		static bool CreateNewWindow(const std::string& name, int width, int height);
		static GlfwWindow* GetWindow();

		static DirectX::SimpleMath::Vector2 GetWindowSize();
	private:
		static void ResizeCallBack(GLFWwindow* window, int width, int height);

		static Application s_Instance;
		GlfwWindow m_Window;
	};
}
