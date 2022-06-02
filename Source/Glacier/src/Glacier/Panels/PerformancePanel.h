#pragma once
#include "Alpine/DX11/Utilities.h"

namespace Alpine
{
	class PerformancePanel
	{
	public:
		void OnRenderImGui();

		static Ref<PerformancePanel> Create();
	private:
	};
}