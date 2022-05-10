#include "DirectionalLight.h"

Alpine::DirectionalLight::DirectionalLight() : m_direction(-1, -1, -1)
{
	m_Color = Vector4(1, 1, 1, 2);
}

Alpine::Ref<Alpine::DirectionalLight> Alpine::DirectionalLight::Create()
{
	return std::make_shared<DirectionalLight>();
}
