#pragma once
#include <d3d11.h>
#include <string>

namespace Alpine
{
	class IWindow
	{
	protected:
		~IWindow() = default;
	public:
		virtual bool InitWindow(const std::string& title, int width, int height) = 0;
		virtual void* GetWindow() = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual float GetAspectRatio() = 0;
		virtual void SetWindowSize(unsigned int width, unsigned int height) = 0;
	};
}
