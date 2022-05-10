#pragma once
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;
class PerspectiveCamera
{
public:
	void Init(const Vector3& aPosition);
	void Update(float aDeltaTime);
	void SetAspectRatio(float ratio);
	Vector3& GetPosition();
	Matrix& GetProjectionMatrix();
	Matrix& GetViewMatrix();
private:
	void CalcMouseMovement();
	void UpdateVectors();
	void UpdateViewMatrix();
	Matrix m_Transform;
	Matrix m_ProjectionMatrix;
	Matrix m_ViewMatrix;
	Vector3 m_Position;
	Vector3 m_Forward;
	Vector3 m_Up;
	Vector3 m_Right;
	Vector3 m_Rotation;
	Vector2 m_LastMousePosition;
	bool m_GiveControl;
	float m_MouseSensitivity;
};