#include "Application.h"
#include "DX11/DX11.h"

Application Application::myInstance;
Application::Application()
{
	myInstance = *this;
}

bool Application::CreateNewWindow(int width, int height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	myInstance.myWindow.window = glfwCreateWindow(width, height, "Alpine", NULL, NULL);
	if (!myInstance.myWindow.window)
	{
		glfwTerminate();
		return false;
	}
	glfwSetWindowSizeCallback(myInstance.myWindow.window, ResizeCallBack);

	return true;
}
GLFWwindow* Application::GetWindow()
{
	return myInstance.myWindow.window;
}

float Application::GetAspectRatio()
{
	glfwGetWindowSize(myInstance.myWindow.window, &myInstance.myWindow.screenWidth, &myInstance.myWindow.screenHeight);
	return static_cast<float>(myInstance.myWindow.screenWidth) / static_cast<float>(myInstance.myWindow.screenHeight);
}

DirectX::SimpleMath::Vector2 Application::GetWindowSize()
{
	return DirectX::SimpleMath::Vector2(myInstance.myWindow.screenWidth, myInstance.myWindow.screenHeight);
}

void Application::ResizeCallBack(GLFWwindow* window, int width, int height)
{
	Alpine::DX11::Resize(width, height);
}
