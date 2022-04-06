#pragma once
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;
namespace Alpine
{
	class Light
	{
	public:
		Vector4 GetLightColor() { return myColor; };
		void SetLightColor(Vector4 color) { myColor = color; };
	protected:
		Vector4 myColor;
	};
}