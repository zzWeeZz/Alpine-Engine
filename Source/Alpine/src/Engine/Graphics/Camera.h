#pragma once
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;
class Camera
{
public:
	void Init(const Vector3& aPosition);
	void Update(float aDeltaTime);
	void SetAspectRatio(float ratio);
	Matrix& GetTransform();
	Matrix& GetProjectionMatrix();
	Matrix& GetViewMatrix();
private:
	void CalcMouseMovement();
	void UpdateVectors();
	void UpdateViewMatrix();
	Matrix myTransform;
	Matrix myProjectionMatrix;
	Matrix myViewMatrix;
	Vector3 myPosition;
	Vector3 myForward;
	Vector3 myUp;
	Vector3 myRight;
	Vector3 myRotation;
	Vector2 myLastMousePosition;
	bool myGiveControll;
	float myMouseSensitivity;
};