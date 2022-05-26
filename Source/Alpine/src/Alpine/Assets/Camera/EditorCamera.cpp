#include "EditorCamera.h"

#include "ToolBox/Input/Input.h"

Alpine::EditorCamera::EditorCamera(const Vector3& aPosition)
{
	m_Position = aPosition;
	m_Projection = Matrix::CreatePerspectiveFieldOfView(3.14f / 2.f, 16.f / 9.f, 0.1f, 1000000.f);
	m_Forward = Vector3(0.f, 0.f, -1.f);
	m_Right = Vector3(-1.f, 0.f, 0.f);
	m_Up = Vector3(0.f, 1.f, 0.f);
	m_Rotation = Vector3(0.f, 0.f, 0.f);
	m_MouseSensitivity = 0.001f;
	m_AspectRatio = 16.f / 9.f;
	m_FOV = 3.14f / 2.f;
	UpdateViewMatrix();
}

void Alpine::EditorCamera::Update(float aDeltaTime)
{
	if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{

		if (Input::GetInstance().GetKeyDown(Keys::A))
		{
			m_Position -= m_Right * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::D))
		{
			m_Position += m_Right * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::W))
		{
			m_Position += m_Forward * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::S))
		{
			m_Position -= m_Forward * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::Space))
		{
			m_Position += m_Up * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		if (Input::GetInstance().GetKeyDown(Keys::Z))
		{
			m_Position -= m_Up * 100 * aDeltaTime;
			UpdateViewMatrix();
		}
		CalcMouseMovement();
	}
	else
	{
		m_GiveControl = false;
	}
}

void Alpine::EditorCamera::SetAspectRatio(float ratio)
{
	m_Projection = Matrix::CreatePerspectiveFieldOfView(m_FOV, ratio, 0.1f, 1000.f);
	m_AspectRatio = ratio;
	UpdateViewMatrix();
}


Vector3& Alpine::EditorCamera::GetPosition()
{
	return m_Position;
}

Matrix& Alpine::EditorCamera::GetProjectionMatrix()
{
	return m_Projection;
}

Matrix& Alpine::EditorCamera::GetViewMatrix()
{
	return m_ViewMatrix;
}

Alpine::Ref<Alpine::EditorCamera> Alpine::EditorCamera::Create(const Vector3& aPosition)
{
	return CreateRef<EditorCamera>(aPosition);
}

void Alpine::EditorCamera::CalcMouseMovement()
{
	if(!m_GiveControl)
	{
		m_LastMousePosition.x = Input::GetInstance().myMousePosition.first;
		m_LastMousePosition.y = Input::GetInstance().myMousePosition.second;
		m_GiveControl = true;
	}
	float xOffset = Input::GetInstance().myMousePosition.first - m_LastMousePosition.x;
	float yOffset = m_LastMousePosition.y - Input::GetInstance().myMousePosition.second;
	m_LastMousePosition.x = Input::GetInstance().myMousePosition.first;
	m_LastMousePosition.y = Input::GetInstance().myMousePosition.second;

	xOffset *= m_MouseSensitivity;
	yOffset *= m_MouseSensitivity;
	m_Rotation = m_Rotation + Vector3(yOffset, -xOffset, 0);

	if (m_Rotation.x > 89.f)
	{
		m_Rotation.x = 89.f;
	}
	if (m_Rotation.x < -89.f)
	{
		m_Rotation.x = -89.f;
	}
	m_Rotation.z = 0;
	UpdateVectors();

}

void Alpine::EditorCamera::UpdateVectors()
{
	auto rotMatrix = Matrix::CreateFromYawPitchRoll(m_Rotation);
	m_Forward = rotMatrix.Forward();
	m_Forward.Normalize();

	m_Right = rotMatrix.Right();
	m_Right.Normalize();

	m_Up = rotMatrix.Up();
	m_Up.Normalize();

	UpdateViewMatrix();
}

void Alpine::EditorCamera::UpdateViewMatrix()
{
	m_ViewMatrix = Matrix::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
}
