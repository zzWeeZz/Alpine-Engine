#include "PointLight.h"

Alpine::PointLight::PointLight() : m_Position(0.f,0.f,0.f, 0.f), m_Falloff(0.3f), m_Range(1.f)
{
}

Alpine::Ref<Alpine::PointLight> Alpine::PointLight::Create()
{
	return CreateRef<PointLight>();
}
