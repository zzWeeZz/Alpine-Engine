#include "GlfwWindow.h"

#include <GLFW/glfw3native.h>

bool Alpine::GlfwWindow::InitWindow(const std::string& title, int width, int height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	myWindow = glfwCreateWindow(width, height, "Alpine", NULL, NULL);
	myWidth = width;
	myHeight = height;
	if (!myWindow)
	{
		glfwTerminate();
		return false;
	}
	return true;
}

void* Alpine::GlfwWindow::GetWindow()
{
	return myWindow;
}

unsigned Alpine::GlfwWindow::GetWidth()
{
	return myWidth;
}

unsigned Alpine::GlfwWindow::GetHeight()
{
	return myHeight;
}

HWND Alpine::GlfwWindow::GetHwnd()
{
	return glfwGetWin32Window(myWindow);
}

float Alpine::GlfwWindow::GetAspectRatio()
{
	return static_cast<float>(myWidth) / static_cast<float>(myHeight);
}

void Alpine::GlfwWindow::SetWindowSize(unsigned width, unsigned height)
{
	myWidth = width;
	myHeight = height;
	glfwSetWindowSize(myWindow, width, height);
}
