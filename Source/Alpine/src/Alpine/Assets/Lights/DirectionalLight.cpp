#include "DirectionalLight.h"

Alpine::DirectionalLight::DirectionalLight() : m_direction(-1, -1, -1)
{
	m_Color = Vector4(1, 1, 1, 2);
	m_LightProjection = Matrix::CreateOrthographic(500, 500, 0.001f, 7500.0);
	auto lightView = Matrix::CreateLookAt({ 10000,10000,10000 }, { 0,0,0 }, { 0,1,0 });
	m_LightSpaceMatrix = lightView * m_LightProjection;
}

void Alpine::DirectionalLight::ValidateLightMatrix(const Vector3& rotation)
{
	auto lightView = Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
	Vector3 dir = lightView.Forward();
	dir.Normalize();
	/*dir *= -1;*/
	auto newMatrix = Matrix::CreateLookAt(dir * 1000, { 0,0,0 }, { 0,1,0 });
	m_LightSpaceMatrix = newMatrix * m_LightProjection;
}

Alpine::Ref<Alpine::DirectionalLight> Alpine::DirectionalLight::Create()
{
	return std::make_shared<DirectionalLight>();
}
