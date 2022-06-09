#pragma once
#include "Alpine/RenderSystem/FrameBuffer.h"

namespace Alpine
{
	struct RenderPass
	{
		Ref<FrameBuffer> frameBuffer;
		std::string ShaderKey;
	};
}