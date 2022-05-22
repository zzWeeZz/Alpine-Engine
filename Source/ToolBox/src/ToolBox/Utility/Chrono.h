#pragma once
#include "Timer.h"

namespace ToolBox
{
	class Chrono
	{
	public:
		
		static float Timestep();
		static float TotalTime();

		static void UpdateTimeData();

	private:
		static ToolBox::Utility::Timer m_Timer;
	};


}


