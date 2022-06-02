#pragma once
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;
#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 8


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
		struct DirLight
		{
			Vector4 lightDirection;
			Vector4 lightColor;
		} DirLightData[MAX_DIR_LIGHTS];
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
		} PointLightData[MAX_POINT_LIGHTS];
	
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