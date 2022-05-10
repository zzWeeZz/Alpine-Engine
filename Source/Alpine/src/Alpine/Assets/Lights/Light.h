#pragma once
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;
namespace Alpine
{
	class Light
	{
	public:
		Vector4 GetLightColor() { return m_Color; };
		void SetLightColor(Vector4 color) { m_Color = color; };
	protected:
		Vector4 m_Color;
	};
}