#pragma once
#include "Mesh.h"
#include <string>
#include <DirectXTK/SimpleMath.h>
#include "DX11/DX11.h"
using namespace DirectX::SimpleMath;

namespace Alpine
{
	class Model
	{
	public:
		void SetModel(std::string aPath, std::wstring aTexturePath = L"Textures/Default.png");
		void Draw();

		Matrix& GetTransform();

	private:
		void PrepareForRender();
		bool LoadModel(const std::string& aFilePath);
		void ProcessNode(aiNode* aNode, const aiScene* aScene);
		Mesh ProcessMesh(aiMesh* aMesh, const aiScene* aScene);

		std::vector<Mesh> myMeshes;

		Matrix myTransform;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
		Microsoft::WRL::ComPtr< ID3D11SamplerState> myTextureSamplerState;
	};
}
