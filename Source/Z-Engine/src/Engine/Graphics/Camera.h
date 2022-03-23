#pragma once
#include "../../ToolBox/src/ToolBox/Math/Transform.h"


class Camera
{
public:
	void Init(const Vector3f& aPosition);
	ToolBox::Math::Transform& GetTransform();
	Matrix4x4f& GetProjectionMatrix();
	Matrix4x4f& GetViewMatrix();
private:
	ToolBox::Math::Transform myTransform;
	Matrix4x4f myProjectionMatrix;
	Matrix4x4f myViewMatrix;

};