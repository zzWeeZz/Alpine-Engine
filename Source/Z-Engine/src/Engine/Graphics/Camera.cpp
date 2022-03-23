#include "Camera.h"

void Camera::Init(const Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
	myProjectionMatrix = Matrix4x4f::CreateLeftHandPerspectiveMatrix(100, 0.1f, 1000.f);
	myViewMatrix = Matrix4x4f::CreateLeftHandLookAtMatrix(myTransform.GetPosition(), {1, 0, 0}, {0, 1, 0});
}

ToolBox::Math::Transform& Camera::GetTransform()
{
	return myTransform;
}

Matrix4x4f& Camera::GetProjectionMatrix()
{
	return myProjectionMatrix;
}

Matrix4x4f& Camera::GetViewMatrix()
{
	return myViewMatrix;
}
