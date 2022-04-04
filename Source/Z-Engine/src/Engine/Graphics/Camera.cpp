#include "Camera.h"

void Camera::Init(const Vector3& aPosition)
{
	myTransform.CreateTranslation(aPosition);
	myProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(3.14f/2.f, 16.f / 9.f, 0.1f, 1000.f);
	myViewMatrix = Matrix::CreateLookAt(myTransform.Translation(), myTransform.Translation() + myTransform.Forward(),myTransform.Up());
}

Matrix& Camera::GetTransform()
{
	return myTransform;
}

Matrix& Camera::GetProjectionMatrix()
{
	return myProjectionMatrix;
}

Matrix& Camera::GetViewMatrix()
{
	return myViewMatrix;
}