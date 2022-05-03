#include "Application.h"
#include "Engine/DX11/DX11.h"

Alpine::Application Alpine::Application::s_Instance;
Alpine::Application::Application()
{
	s_Instance = *this;
}

bool Alpine::Application::CreateNewWindow(const std::string& name, int width, int height)
{
	s_Instance.m_Window.InitWindow(name, width, height);
	glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(s_Instance.m_Window.GetWindow()), ResizeCallBack);

	return true;
}
Alpine::GlfwWindow* Alpine::Application::GetWindow()
{
	return &s_Instance.m_Window;
}

void Alpine::Application::Run()
{

}

void Alpine::Application::ResizeCallBack(GLFWwindow* window, int width, int height)
{
	s_Instance.m_Window.SetWindowSize(width, height);
	Alpine::DX11::Resize(width, height);
}
