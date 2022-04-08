#pragma once
#include <GLFW/glfw3.h>
#include "IWindow.h"
#define GLFW_EXPOSE_NATIVE_WGL 
#define GLFW_EXPOSE_NATIVE_WIN32

namespace Alpine
{
	class GlfwWindow : public IWindow
	{
	public:
		void* GetWindow() override;
		unsigned int GetWidth() override;
		unsigned int GetHeight() override;
		float GetAspectRatio() override;
		void SetWindowSize(unsigned int width, unsigned int height) override;
		virtual ~GlfwWindow() = default;
	private:
		GLFWwindow* myWindow;
		unsigned int myWidth;
		unsigned int myHeight;
	};
}
