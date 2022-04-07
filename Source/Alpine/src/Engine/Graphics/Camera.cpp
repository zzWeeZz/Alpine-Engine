#include "Camera.h"

#include "ToolBox/Input/Input.h"
#include "ToolBox/Math/Matrix/Matrix3x3.hpp"

void Camera::Init(const Vector3& aPosition)
{
	myTransform.CreateTranslation(aPosition);
	myProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(3.14f / 2.f, 16.f / 9.f, 0.1f, 1000.f);
	myForward = Vector3(0.f, 0.f, -1.f);
	myRight = Vector3(-1.f, 0.f, 0.f);
	myUp = Vector3(0.f, 1.f, 0.f);
	myRotation = Vector3(0.f, 0.f, 0.f);
	myMouseSensitivity = 0.001f;
	UpdateViewMatrix();
}

void Camera::Update(float aDeltaTime)
{
	if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{

		if (Input::GetInstance().GetKeyDown(Keys::A))
		{
			myPosition -= myRight * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::D))
		{
			myPosition += myRight * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::W))
		{
			myPosition += myForward * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::S))
		{
			myPosition -= myForward * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::Space))
		{
			myPosition += myUp * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::Z))
		{
			myPosition -= myUp * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		CalcMouseMovement();
	}
	else
	{
		myGiveControll = false;
	}
}

void Camera::SetAspectRatio(float ratio)
{
	myProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(3.14f / 2.f, ratio, 0.1f, 1000.f);
	UpdateViewMatrix();
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

void Camera::CalcMouseMovement()
{
	if(!myGiveControll)
	{
		myLastMousePosition.x = Input::GetInstance().myMousePosition.first;
		myLastMousePosition.y = Input::GetInstance().myMousePosition.second;
		myGiveControll = true;
	}
	float xOffset = Input::GetInstance().myMousePosition.first - myLastMousePosition.x;
	float yOffset = myLastMousePosition.y - Input::GetInstance().myMousePosition.second;
	myLastMousePosition.x = Input::GetInstance().myMousePosition.first;
	myLastMousePosition.y = Input::GetInstance().myMousePosition.second;

	xOffset *= myMouseSensitivity;
	yOffset *= myMouseSensitivity;
	myRotation = myRotation + Vector3(yOffset, -xOffset, 0);

	if (myRotation.x > 89.f)
	{
		myRotation.x = 89.f;
	}
	if (myRotation.x < -89.f)
	{
		myRotation.x = -89.f;
	}
	myRotation.z = 0;
	UpdateVectors();

}

void Camera::UpdateVectors()
{
	auto rotMatrix = Matrix::CreateFromYawPitchRoll(myRotation);
	myForward = rotMatrix.Forward();
	myForward.Normalize();

	myRight = rotMatrix.Right();
	myRight.Normalize();

	myUp = rotMatrix.Up();
	myUp.Normalize();

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	myViewMatrix = Matrix::CreateLookAt(myPosition, myPosition + myForward, myUp);
}
