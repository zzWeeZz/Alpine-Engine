#include "Mesh.h"

#include <future>
#include <iostream>
#include <spdlog/spdlog.h>
#include <DirectXTK/GeometricPrimitive.h>

#include "Alpine/Assets/Materials/Material.h"

void Alpine::Mesh::SetMesh(std::string aPath)

{
	auto device = DX11::Device();
	auto context = DX11::Context();
	if (aPath == "Cube")
	{
		DirectX::GeometricPrimitive::VertexCollection verts;
		DirectX::GeometricPrimitive::IndexCollection indences;
		DirectX::GeometricPrimitive::CreateBox(verts, indences, { 1, 1, 1 }, false, false);
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;
		for (size_t i = 0; i < verts.size(); i++)
		{
			Vertex tempV;
			tempV.pos = verts[i].position;
			tempV.texCoord = verts[i].textureCoordinate;
			tempV.norm = verts[i].normal;
			vertices.push_back(tempV);
		}
		for (size_t i = 0; i < indences.size(); i++)
		{
			indices.push_back(indences[i]);
		}
		//m_SubMeshes.push_back(SubMesh(vertices, indices));
	}
	else if (aPath == "Sphere")
	{
		DirectX::GeometricPrimitive::VertexCollection verts;
		DirectX::GeometricPrimitive::IndexCollection indences;
		DirectX::GeometricPrimitive::CreateGeoSphere(verts, indences, 1, 3, true);
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;
		for (size_t i = 0; i < verts.size(); i++)
		{
			Vertex tempV;
			tempV.pos = verts[i].position;
			tempV.texCoord = verts[i].textureCoordinate;
			tempV.norm = verts[i].normal;
			vertices.push_back(tempV);
		}
		for (size_t i = 0; i < indences.size(); i++)
		{
			indices.push_back(indences[i]);
		}
		//m_SubMeshes.push_back(SubMesh(vertices, indices));
	}
	else
	{
		LoadModel(aPath);
	}

	PrepareForRender();
}

void Alpine::Mesh::SubmitMesh()
{
	for (auto& subMesh : m_SubMeshes)
	{
		subMesh.Draw();
	}
}

void Alpine::Mesh::PrepareForRender()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX11::Device()->CreateSamplerState(&samplerDesc, &m_TextureSamplerState);
}

void Alpine::Mesh::ProcessMaterials(const aiScene* pScene, const std::filesystem::path& path)
{
	auto dir = path.parent_path();
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{

		Ref<Material> tempMaterial = Material::Create("dflak");
		{
			std::filesystem::path diffuseColor = dir.string() + "/" + pScene->mMaterials[i]->GetName().C_Str() + "_c.dds";
			if (std::filesystem::exists(diffuseColor))
			{
				tempMaterial->AddTexture(Texture::Create(diffuseColor));
			}
		}
		{
			std::filesystem::path diffuseColor = dir.string() + "/" + pScene->mMaterials[i]->GetName().C_Str() + "_m.dds";
			if (std::filesystem::exists(diffuseColor))
			{
				tempMaterial->AddTexture(Texture::Create(diffuseColor));
			}
		}
		{
			std::filesystem::path diffuseColor = dir.string() + "/" + pScene->mMaterials[i]->GetName().C_Str() + "_n.dds";
			if (std::filesystem::exists(diffuseColor))
			{
				tempMaterial->AddTexture(Texture::Create(diffuseColor));
			}
		}
		{
			std::filesystem::path diffuseColor = dir.string() + "C.dds";
			if (std::filesystem::exists(diffuseColor))
			{
				tempMaterial->AddTexture(Texture::Create(diffuseColor));
			}
		}
		{
			std::filesystem::path diffuseColor = dir.string() + "C.dds";
			if (std::filesystem::exists(diffuseColor))
			{
				tempMaterial->AddTexture(Texture::Create(diffuseColor));
			}
		}
		{
			std::filesystem::path diffuseColor = dir.string() + "C.dds";
			if (std::filesystem::exists(diffuseColor))
			{
				tempMaterial->AddTexture(Texture::Create(diffuseColor));
			}
		}
		m_Materials.push_back(tempMaterial);
	}
}

bool Alpine::Mesh::LoadModel(const std::string& aFilePath)
{
	Assimp::Importer importer;
	spdlog::log(spdlog::level::info, "Loading: {}", aFilePath);
	const aiScene* pScene = importer.ReadFile(aFilePath, aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_SortByPType |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_OptimizeMeshes |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ValidateDataStructure |
		aiProcess_FlipUVs);

	if (pScene == nullptr)
	{
		spdlog::error("Failed to load model: {}", aFilePath);
		return false;
	}
	ProcessMaterials(pScene, aFilePath);
	ProcessNode(pScene->mRootNode, pScene);
	spdlog::log(spdlog::level::info, "Done loading: {}", aFilePath);
	return true;
}

void Alpine::Mesh::ProcessNode(aiNode* aNode, const aiScene* aScene)
{
	for (int i = 0; i < aNode->mNumMeshes; i++)
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];

		m_SubMeshes.push_back(ProcessSubMesh(mesh, aScene));
	}

	for (int i = 0; i < aNode->mNumChildren; i++)
	{
		ProcessNode(aNode->mChildren[i], aScene);
	}
}

Alpine::SubMesh Alpine::Mesh::ProcessSubMesh(aiMesh* aMesh, const aiScene* aScene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	if (aMesh->mTextureCoords[0])
	{
		for (int i = 0; i < aMesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.pos.x = aMesh->mVertices[i].x;
			vertex.pos.y = aMesh->mVertices[i].y;
			vertex.pos.z = aMesh->mVertices[i].z;


			vertex.texCoord.x = (float)aMesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)aMesh->mTextureCoords[0][i].y;


			vertex.norm.x = aMesh->mNormals[i].x;
			vertex.norm.y = aMesh->mNormals[i].y;
			vertex.norm.z = aMesh->mNormals[i].z;

			vertex.tang.x = aMesh->mTangents[i].x;
			vertex.tang.y = aMesh->mTangents[i].y;
			vertex.tang.z = aMesh->mTangents[i].z;

			vertex.biTang.x = aMesh->mBitangents[i].x;
			vertex.biTang.y = aMesh->mBitangents[i].y;
			vertex.biTang.z = aMesh->mBitangents[i].z;

			vertices.push_back(vertex);
		}
	}


	for (int i = 0; i < aMesh->mNumFaces; i++)
	{
		aiFace face = aMesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	return SubMesh(vertices, indices, m_Materials[aMesh->mMaterialIndex]);
}
