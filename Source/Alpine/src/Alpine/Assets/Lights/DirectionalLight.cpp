#include "DirectionalLight.h"

Alpine::DirectionalLight::DirectionalLight() : m_direction(-1, -1, -1)
{
	m_Color = Vector4(1, 1, 1, 2);
	auto lightProjection = Matrix::CreateOrthographic(1000, 1000, 1.0f, 7500.0);
	auto lightView = Matrix::CreateLookAt({ 1000,1000,1000 }, { 0,0,0 }, { 0,1,0 });
	m_LightSpaceMatrix = lightView * lightProjection;
}

Alpine::Ref<Alpine::DirectionalLight> Alpine::DirectionalLight::Create()
{
	return std::make_shared<DirectionalLight>();
}
