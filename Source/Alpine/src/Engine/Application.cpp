#include "Application.h"
#include "DX11/DX11.h"

Window Application::myWindow;
bool Application::CreateNewWindow(int width, int height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	Application::myWindow.window = glfwCreateWindow(width, height, "Alpine", NULL, NULL);
	if (!Application::myWindow.window)
	{
		glfwTerminate();
		return false;
	}
	glfwSetWindowSizeCallback(Application::myWindow.window, ResizeCallBack);

	return true;
}
GLFWwindow* Application::GetWindow()
{
	return myWindow.window;
}

float Application::GetAspectRatio()
{
	glfwGetWindowSize(Application::myWindow.window, &Application::myWindow.screenWidth, &Application::myWindow.screenHeight);
	return static_cast<float>(Application::myWindow.screenWidth) / static_cast<float>(Application::myWindow.screenHeight);
}

std::pair<int, int> Application::GetWindowSize()
{
	glfwGetWindowSize(Application::myWindow.window, &Application::myWindow.screenWidth, &Application::myWindow.screenHeight);
	return std::pair<int, int>(Application::myWindow.screenWidth, Application::myWindow.screenHeight);
}

void Application::ResizeCallBack(GLFWwindow* window, int width, int height)
{
	Alpine::DX11::Resize(width, height);
}
