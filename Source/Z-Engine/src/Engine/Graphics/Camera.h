#pragma once
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;
class Camera
{
public:
	void Init(const Vector3& aPosition);
	Matrix& GetTransform();
	Matrix& GetProjectionMatrix();
	Matrix& GetViewMatrix();
private:
	Matrix myTransform;
	Matrix myProjectionMatrix;
	Matrix myViewMatrix;

};