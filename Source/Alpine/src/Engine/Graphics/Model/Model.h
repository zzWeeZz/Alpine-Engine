#pragma once
#include "SubMesh.h"

#include <string>
#include <DirectXTK/SimpleMath.h>
#include "Mesh.h"
#include "Materials/Material.h"

using namespace DirectX::SimpleMath;

namespace Alpine
{
	class Model
	{
	public:
		void LoadModel(std::string aPath, std::shared_ptr<Material> material);
		void Draw();

		Matrix& GetTransform();
		Vector3 GetPosition();
		Vector3 GetRotation();
		Vector3 GetScale();
		void SetPosition(const Vector3& aPosition);
		void SetRotation(const Vector3& aRotation);
		void Rotate(const Vector3& aRotation);
		void SetScale(const Vector3& aScale);

	private:
		void CalculateTransform();

		Vector3 myPosition;
		Vector3 myRotation;
		Vector3 mySize;

		std::shared_ptr<Material> myMaterial;

		Mesh myMesh;
		Matrix myTransform;
	};
}
