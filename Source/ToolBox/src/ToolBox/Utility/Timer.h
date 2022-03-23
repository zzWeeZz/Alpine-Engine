#pragma once
#include <chrono>
#include <iostream>

namespace ToolBox
{
	namespace Utility
	{
		class Timer
		{
		public:
			Timer();
			Timer(const Timer& aTimer) = delete;
			Timer& operator=(const Timer& aTimer) = delete;
			void Update();
			float GetDeltaTime() const;
			double GetTotalTime() const;
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> myPreviusTime;
			std::chrono::duration<float> myDeltaTime;
			std::chrono::duration<double> myTotalTime;
		};
	}
}