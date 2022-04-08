#pragma once

namespace Alpine
{
	class IWindow
	{
	protected:
		~IWindow() = default;
	public:
		virtual void* GetWindow() = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual float GetAspectRatio() = 0;
		virtual void SetWindowSize(unsigned int width, unsigned int height) = 0;
	};
}