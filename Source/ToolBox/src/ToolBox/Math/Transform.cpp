#include "Transform.h"

void ToolBox::Math::Transform::SetPosition(const Vector3f& aPosition)
{
	myPosition = aPosition;
	myTransform *= Matrix4x4f::CreateTranslation(aPosition);
	SetSize({ 1.f, 1.f, 1.f });
}

void ToolBox::Math::Transform::SetRotation(Vector3f aRotation)
{
	myRotation = aRotation;
	myTransform *= Matrix4x4f::CreateRotationInLocalSpace(myTransform, aRotation.Length(), aRotation);
}

void ToolBox::Math::Transform::SetSize(const Vector3f& aSize)
{
	mySize = aSize;
	myTransform *= Matrix4x4f::CreateScaleMatrix(aSize);
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
