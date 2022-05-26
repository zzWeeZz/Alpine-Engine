#include "SceneCamera.h"

Alpine::SceneCamera::SceneCamera()
{
	RebuildAllMatrices();
}

void Alpine::SceneCamera::SetPosition(const Vector3& position)
{
	m_Position = position;
	RebuildTransform();
}

void Alpine::SceneCamera::SetRotation(const Vector3& rotation)
{
	m_Rotation = rotation;
	RebuildTransform();
}

void Alpine::SceneCamera::SetFov(const float& fov)
{
	m_Fov = fov;
	RebuildProjection();
}

Alpine::Ref<Alpine::SceneCamera> Alpine::SceneCamera::Create()
{
	return CreateRef<SceneCamera>();
}

void Alpine::SceneCamera::RebuildTransform()
{
	auto rotMatrix = Matrix::CreateFromYawPitchRoll(m_Rotation);
	m_Forward = rotMatrix.Forward();
	m_Forward.Normalize();

	m_Right = rotMatrix.Right();
	m_Right.Normalize();

	m_Up = rotMatrix.Up();
	m_Up.Normalize();

	RebuildView();
}

void Alpine::SceneCamera::RebuildView()
{
	m_ViewMatrix = Matrix::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
}

void Alpine::SceneCamera::RebuildProjection()
{
	m_Projection = Matrix::CreatePerspectiveFieldOfView(m_Fov, 16.f / 9.f, 1.0f, 10000.f);
}

void Alpine::SceneCamera::RebuildAllMatrices()
{
	RebuildTransform();
	RebuildProjection();
}
