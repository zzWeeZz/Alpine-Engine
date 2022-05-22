#include "Chrono.h"

ToolBox::Utility::Timer ToolBox::Chrono::m_Timer;
float ToolBox::Chrono::Timestep()
{
	return m_Timer.GetDeltaTime();
}

float ToolBox::Chrono::TotalTime()
{
	return static_cast<float>(m_Timer.GetTotalTime());
}

void ToolBox::Chrono::UpdateTimeData()
{
	m_Timer.Update();
}
