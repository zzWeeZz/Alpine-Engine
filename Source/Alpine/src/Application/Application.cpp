#include "Application.h"
#include "DX11/DX11.h"

Alpine::Application Alpine::Application::myInstance;
Alpine::Application::Application()
{
	myInstance = *this;
}

bool Alpine::Application::CreateNewWindow(const std::string& name, int width, int height)
{
	myInstance.myWindow.InitWindow(name, width, height);
	glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(myInstance.myWindow.GetWindow()), ResizeCallBack);

	return true;
}
Alpine::GlfwWindow* Alpine::Application::GetWindow()
{
	return &myInstance.myWindow;
}


DirectX::SimpleMath::Vector2 Alpine::Application::GetWindowSize()
{
	return DirectX::SimpleMath::Vector2(myInstance.myWindow.GetWidth(), myInstance.myWindow.GetHeight());
}

void Alpine::Application::ResizeCallBack(GLFWwindow* window, int width, int height)
{
	myInstance.myWindow.SetWindowSize(width, height);
	Alpine::DX11::Resize(width, height);
}
