#include "Timer.h"

namespace ToolBox
{
	namespace Utility
	{
		Timer::Timer() : myDeltaTime(0), myTotalTime(0)
		{
			myPreviusTime = std::chrono::high_resolution_clock::now();
		}

		void Timer::Update()
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsed = currentTime - myPreviusTime;

			myDeltaTime = elapsed;
			myTotalTime += elapsed;
			myPreviusTime = currentTime;
		}

		float Timer::GetDeltaTime() const
		{
			return myDeltaTime.count();
		}

		double Timer::GetTotalTime() const
		{
			return myTotalTime.count();
		}
	}
}