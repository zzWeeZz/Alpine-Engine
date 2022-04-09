#include "Model.h"
#include <iostream>
#include <DirectXTK/GeometricPrimitive.h>
#include <spdlog/spdlog.h>
#include "DX11/DX11.h"
#include "ToolBox/Utility/UtilityFunctions.hpp"

void Alpine::Model::LoadModel(std::string aPath, std::wstring aTexturePath)
{
	myMaterial = Material::Create("Default");
	myMesh.SetMesh(aPath, aTexturePath);
	myPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	myRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	mySize = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void Alpine::Model::Draw()
{
	myMaterial->Bind();
	myMesh.SubmitMesh();
}

Matrix& Alpine::Model::GetTransform()
{
	return myTransform;
}

void Alpine::Model::SetPosition(const Vector3& aPosition)
{
	myPosition = aPosition;
	CalculateTransform();
}

void Alpine::Model::SetRotation(const Vector3& aRotation)
{
	myRotation = aRotation / 180.0f * DirectX::XM_PI;
	CalculateTransform();
}

void Alpine::Model::Rotate(const Vector3& aRotation)
{
	Vector3 vec = aRotation / 180.0f * DirectX::XM_PI;
	myRotation += vec;
	CalculateTransform();
}

void Alpine::Model::SetScale(const Vector3& aScale)
{
	mySize = aScale;
	CalculateTransform();
}

void Alpine::Model::CalculateTransform()
{
	Matrix transform;
	transform *= Matrix::CreateRotationX(myRotation.x);
	transform *= Matrix::CreateRotationZ(myRotation.z);
	transform *= Matrix::CreateRotationY(myRotation.y);
	transform *= Matrix::CreateScale(mySize.x, mySize.y, mySize.z);
	transform *= Matrix::CreateTranslation(myPosition.x, myPosition.y, myPosition.z);
	myTransform = transform;
}
