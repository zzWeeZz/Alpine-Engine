#pragma once
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace Alpine
{
	struct CameraBuffer
	{
		Vector4 position;
		Matrix toCameraSpace;
		Matrix toProjectionSpace;
	};
	struct DirectionalLightBuffer
	{
		Vector4 lightColor;
		Vector4 lightDirection;
	};
	struct LightBuffer
	{
		DirectionalLightBuffer lights[4];
		Vector4 ambientColor;
	};
}