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
		Model(const Model& model);
		void Draw(CullMode cullmode = CullMode::Back, DepthStencilMode depthMode = DepthStencilMode::ReadWrite);
		Matrix& GetTransform();
		Vector3 GetPosition();
		Vector3 GetRotation();
		Vector3 GetScale();
		void SetPosition(const Vector3& aPosition);
		void SetRotation(const Vector3& aRotation);
		void Rotate(const Vector3& aRotation);
		void SetScale(const Vector3& aScale);
		std::vector<Ref<Material>>& GetMaterials() { return m_Materials; }
		static Ref<Model> Create(const std::filesystem::path& aPath);

	private:
		void CalculateTransform();

		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Size;

		std::vector<Ref<Material>> m_Materials;
		Mesh m_Mesh;
		Matrix m_Transform;
		static std::unordered_map<std::string, Ref<Model>> s_Registry;
	};
}
