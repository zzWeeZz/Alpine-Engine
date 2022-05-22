#pragma once
#include "Light.h"
#include "Alpine/DX11/Utilities.h"

namespace Alpine
{
	class PointLight : public Light
	{
	public:
		PointLight();
		void SetPosition(const Vector3& position) { m_Position = Vector4(position.x, position.y, position.z, 1.0f); }
		Vector3 GetPosition() const { return Vector3(m_Position.x, m_Position.y, m_Position.z); }

		void SetRange(const float& range) { m_Range = range; }
		float GetRange() const { return m_Range; }

		void SetFallOff(const float& falloff) { m_Falloff = falloff; }
		float GetFallOff() { return m_Falloff; }

		static Ref<PointLight> Create();
	private:
		Vector4 m_Position;
		float m_Falloff;
		float m_Range;
	};
}
