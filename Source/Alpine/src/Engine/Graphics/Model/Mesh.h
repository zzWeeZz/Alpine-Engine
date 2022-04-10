#pragma once
#include <future>
#include <string>
#include "SubMesh.h"
#include "Materials/Material.h"

namespace Alpine
{
	class Mesh
	{
	public:
		void SetMesh(std::string aPath, std::shared_ptr<Material>& material);
		void SubmitMesh();

	private:
		void PrepareForRender();
		bool LoadModel(const std::string& aFilePath);
		void ProcessNode(aiNode* aNode, const aiScene* aScene);
		SubMesh ProcessSubMesh(aiMesh* aMesh, const aiScene* aScene);

	private:
		std::vector<SubMesh> mySubMeshes;
		std::shared_ptr<Material> myMaterial;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
		Microsoft::WRL::ComPtr< ID3D11SamplerState> myTextureSamplerState;
		static std::vector<std::future<bool>> myFutures;
	};
}
