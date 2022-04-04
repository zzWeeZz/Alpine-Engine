#include "Transform.h"


//void ToolBox::Math::Transform::SetPosition(const Vector3& aPosition)
//{
//	myTransform *= Matrix::CreateTranslation(aPosition);
//}
//
//void ToolBox::Math::Transform::SetRotation(Vector3 aRotation)
//{
//	myTransform *= Matrix::CreateRotationX(aRotation.x);
//	myTransform *= Matrix::CreateRotationY(aRotation.y);
//	myTransform *= Matrix::CreateRotationZ(aRotation.z);
//}
//
//void ToolBox::Math::Transform::SetSize(const Vector3& aSize)
//{
//	myTransform *= Matrix::CreateScale(aSize);
//}
//
//void ToolBox::Math::Transform::SetLocalPosition(const Vector3& aPosition)
//{
//	myTransform = Matrix::CreateTranslation(aPosition) * myTransform;
//}
//
//const Vector3& ToolBox::Math::Transform::GetPosition() const
//{
//	return myTransform.Translation();
//}
//
//const Vector3& ToolBox::Math::Transform::GetRotation() const
//{
//	return myTransform.ToEuler();
//}
//
//const Vector3& ToolBox::Math::Transform::GetSize() const
//{
//	return { 0,0,0 };
//}
//
//const Vector3& ToolBox::Math::Transform::GetForward() const
//{
//	return myTransform.Forward();
//}
//
//const Vector3& ToolBox::Math::Transform::GetUp() const
//{
//	return myTransform.Up();
//}
//
//const Vector3& ToolBox::Math::Transform::GetRight() const
//{
//	return myTransform.Right();
//}
//
//Matrix& ToolBox::Math::Transform::GetMatrix()
//{
//	return myTransform;
//}
