#pragma once
#include "SubMesh.h"

#include <string>
#include "Mesh.h"
#include "Engine/Materials/Material.h"
#include "Engine/DX11/DX11.h"

namespace Alpine
{
	class Model
	{
	public:
		Model(std::string aPath, Ref<Material> material);
		void Draw(CullMode cullmode = CullMode::Back, DepthStencilMode depthMode = DepthStencilMode::ReadWrite);

		Matrix& GetTransform();
		Vector3 GetPosition();
		Vector3 GetRotation();
		Vector3 GetScale();
		void SetPosition(const Vector3& aPosition);
		void SetRotation(const Vector3& aRotation);
		void Rotate(const Vector3& aRotation);
		void SetScale(const Vector3& aScale);

		static Ref<Model> Create(std::string aPath, Ref<Material> material);

	private:
		void CalculateTransform();

		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Size;

		Ref<Material> m_Material;

		Mesh m_Mesh;
		Matrix m_Transform;
	};
}
