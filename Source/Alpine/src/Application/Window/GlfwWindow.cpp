#include "GlfwWindow.h"

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

float Alpine::GlfwWindow::GetAspectRatio()
{
	return static_cast<float>(myWidth) / static_cast<float>(myHeight);
}

void Alpine::GlfwWindow::SetWindowSize(unsigned width, unsigned height)
{
	glfwSetWindowSize(myWindow, width, height);
}
