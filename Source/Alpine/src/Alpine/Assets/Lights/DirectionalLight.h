#pragma once
#include "Light.h"
#include "Alpine/DX11/Utilities.h"
namespace Alpine
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		void SetDirection(Vector3 direction) { m_direction = direction; }
		Vector3 GetDirection() const { return m_direction; }
		Matrix& GetLightSpaceMatrix() { return m_LightSpaceMatrix; }
		static Ref<DirectionalLight> Create();
	private:
		Matrix m_LightSpaceMatrix;
		Vector3 m_direction;
	};
}