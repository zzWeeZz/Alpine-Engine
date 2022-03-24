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
	Vector4f quatVec = { 1 ,aRotation.x,aRotation.y ,aRotation.z };
	myTransform *= Matrix4x4f::CreateRotationInLocalSpace(myTransform, 0.001f, { 1, 0, 0 });
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
