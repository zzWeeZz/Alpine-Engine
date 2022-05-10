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
		Matrix viewMatrix;
	};

	struct DirLightBuffer
	{
		Vector4 lightDirection;
		Vector4 lightColor;
	};

	struct PointLightBuffer
	{
		struct PointLight
		{
			Vector4 lightPosition;
			Vector4 lightColor;
			float falloff;
			float lightRange;
			float padding[2];
		} PointLightData[8];
	
	};

	struct SpotLightBuffer
	{
		Vector4 lightPosition;
		Vector4 lightColor;
		Vector4 lightDirection;
		Vector4 attenuation;
		float lightRange;
		float lightInnerRadius;
		float lightOuterRadius;
		float padding[1];
	};

	struct SpectularMapFillerSettingsBuffer
	{
		float roughness;
		float padding[3];
	};
}