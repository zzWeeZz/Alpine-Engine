#include "Transform.h"

void ToolBox::Math::Transform::SetPosition(const Vector3f& aPosition)
{
	myPosition = aPosition;
	auto m = Matrix4x4f::CreateTranslation(aPosition);
	for (int i = 1; i <= 3; i++)
	{
		myTransform(4, i) = m(4, i);
	}
}

void ToolBox::Math::Transform::SetRotation(Vector3f aRotation)
{
	myRotation = aRotation;

	auto Xrot = Matrix4x4f::CreateRotationAroundX(aRotation.x);
	auto Yrot = Matrix4x4f::CreateRotationAroundY(aRotation.y);
	auto Zrot = Matrix4x4f::CreateRotationAroundZ(aRotation.z);

	myTransform *= Xrot;
	myTransform *= Yrot;
	myTransform *= Zrot;
}

void ToolBox::Math::Transform::SetSize(const Vector3f& aSize)
{
	mySize = aSize;
	myTransform *= Matrix4x4f::CreateScaleMatrix(aSize);
}

void ToolBox::Math::Transform::SetLocalPosition(const Vector3f& aPosition)
{
	Vector4f posIn4 = { aPosition.x,aPosition.y ,aPosition.z, 1 };

	auto localPos = posIn4 * myTransform;
	auto m = Matrix4x4f::CreateTranslation({ localPos.x,localPos.y ,localPos.z});
	for (int i = 1; i <= 3; i++)
	{
		myTransform(4, i) = m(4, i);
	}
}

const Vector3f& ToolBox::Math::Transform::GetPosition() const
{
	return myPosition;
}

const Vector3f& ToolBox::Math::Transform::GetRotation() const
{
	return myRotation;
}

const Vector3f& ToolBox::Math::Transform::GetSize() const
{
	return mySize;
}

Matrix4x4f& ToolBox::Math::Transform::GetMatrix()
{
	return myTransform;
}
