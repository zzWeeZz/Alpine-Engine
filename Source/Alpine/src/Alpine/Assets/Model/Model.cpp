#include "Model.h"
#include <iostream>
#include <DirectXTK/GeometricPrimitive.h>
#include <spdlog/spdlog.h>
#include "Alpine/DX11/DX11.h"
#include "ToolBox/Utility/UtilityFunctions.hpp"
#include "Alpine/RenderSystem/Renderer.h"
#include "Alpine/RenderSystem/RenderCommands.h"

Alpine::Model::Model(std::string aPath)
{
	m_Mesh.SetMesh(aPath);
	m_Position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Size = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Materials = m_Mesh.m_Materials;
}

void Alpine::Model::Draw(CullMode cullmode, DepthStencilMode depthMode)
{
	m_Mesh.SubmitMesh();
}

Matrix& Alpine::Model::GetTransform()
{
	return m_Transform;
}

Vector3 Alpine::Model::GetPosition()
{
	return m_Position;
}

Vector3 Alpine::Model::GetRotation()
{
	return m_Rotation;
}

Vector3 Alpine::Model::GetScale()
{
	return m_Size;
}

void Alpine::Model::SetPosition(const Vector3& aPosition)
{
	m_Position = aPosition;
	CalculateTransform();
}

void Alpine::Model::SetRotation(const Vector3& aRotation)
{
	m_Rotation = aRotation;
	CalculateTransform();
}

void Alpine::Model::Rotate(const Vector3& aRotation)
{
	Vector3 vec = aRotation / 180.0f * DirectX::XM_PI;
	m_Rotation += vec;
	CalculateTransform();
}

void Alpine::Model::SetScale(const Vector3& aScale)
{
	m_Size = aScale;
	CalculateTransform();
}

Alpine::Ref<Alpine::Model> Alpine::Model::Create(std::string aPath)
{
	return std::make_shared<Model>(aPath);
}

void Alpine::Model::CalculateTransform()
{
	Matrix transform;
	transform *= Matrix::CreateScale(m_Size.x, m_Size.y, m_Size.z);
	transform *= Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	transform *= Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_Transform = transform;
}
