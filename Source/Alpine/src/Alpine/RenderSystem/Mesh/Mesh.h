#pragma once
#include <future>
#include <string>

#include "SubMesh.h"

#include "Alpine/Assets/Materials/Material.h"

namespace Alpine
{
	class Mesh
	{
	public:
		void SetMesh(std::string aPath);
		void SubmitMesh();

	private:
		void PrepareForRender();
		bool LoadModel(const std::string& aFilePath);
		void ProcessNode(aiNode* aNode, const aiScene* aScene);
		SubMesh ProcessSubMesh(aiMesh* aMesh, const aiScene* aScene);

	private:
		std::vector<SubMesh> m_SubMeshes;
		ComPtr<ID3D11ShaderResourceView> m_Texture;
		ComPtr<ID3D11SamplerState> m_TextureSamplerState;
		static std::vector<std::future<bool>> m_Futures;
	};
}
