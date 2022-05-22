#pragma once
#include <string>

#include "Alpine/DX11/DX11.h"

#include "Alpine/RenderSystem/Mesh/Mesh.h"
#include "Alpine/RenderSystem/Mesh/SubMesh.h"

#include "Alpine/Assets/Materials/Material.h"

namespace Alpine
{
	class Model
	{
	public:
		Model(std::string aPath);
		void Draw(CullMode cullmode = CullMode::Back, DepthStencilMode depthMode = DepthStencilMode::ReadWrite);

		Matrix& GetTransform();
		Vector3 GetPosition();
		Vector3 GetRotation();
		Vector3 GetScale();
		void SetPosition(const Vector3& aPosition);
		void SetRotation(const Vector3& aRotation);
		void Rotate(const Vector3& aRotation);
		void SetScale(const Vector3& aScale);

		static Ref<Model> Create(std::string aPath);

	private:
		void CalculateTransform();

		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Size;


		Mesh m_Mesh;
		Matrix m_Transform;
	};
}
