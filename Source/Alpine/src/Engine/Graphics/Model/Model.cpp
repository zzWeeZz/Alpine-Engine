#include "Model.h"
#include <iostream>
#include <DirectXTK/GeometricPrimitive.h>



void Alpine::Model::SetModel(std::string aPath, std::wstring aTexturePath)
{
	auto device = DX11::GetDevice();
	auto context = DX11::GetDeviceContext();
	auto hr = DirectX::CreateWICTextureFromFile(device, aTexturePath.c_str(), nullptr, &myTexture);
	if (FAILED(hr))
	{
		std::cout << "ERROR TEXTURE DIDNT LOAD" << std::endl;
		DirectX::CreateWICTextureFromFile(device, L"Textures/MissingTexture.png", nullptr, &myTexture);
	}
	if (aPath == "Cube")
	{
		DirectX::GeometricPrimitive::VertexCollection verts;
		DirectX::GeometricPrimitive::IndexCollection indences;
		DirectX::GeometricPrimitive::CreateBox(verts, indences, {1, 1, 1}, false, false);
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
		myMeshes.push_back(Mesh(vertices, indices));
	}
	else if (aPath == "Sphere")
	{
		DirectX::GeometricPrimitive::VertexCollection verts;
		DirectX::GeometricPrimitive::IndexCollection indences;
		DirectX::GeometricPrimitive::CreateGeoSphere(verts, indences);
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
		myMeshes.push_back(Mesh(vertices, indices));
	}
	else
	{
		if(!LoadModel(aPath))
		{
			std::cout << "ERROR MODEL DIDNT LOAD!\n";
		}
	}

	PrepareForRender();
}

void Alpine::Model::PrepareForRender()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX11::GetDevice()->CreateSamplerState(&samplerDesc, &myTextureSamplerState);
}

bool Alpine::Model::LoadModel(const std::string& aFilePath)
{
	Assimp::Importer importer;
	
	const aiScene* pScene = importer.ReadFile(aFilePath, 
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_OptimizeMeshes |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ValidateDataStructure |
		aiProcess_FlipUVs);

	if (pScene == nullptr)
	{
		return false;
	}
	ProcessNode(pScene->mRootNode, pScene);
	return true;
}

void Alpine::Model::ProcessNode(aiNode* aNode, const aiScene* aScene)
{
	for (int i = 0; i < aNode->mNumMeshes; i++)
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		myMeshes.push_back(ProcessMesh(mesh, aScene));
	}

	for (int i = 0; i < aNode->mNumChildren; i++)
	{
		ProcessNode(aNode->mChildren[i], aScene);
		
	}
}

Alpine::Mesh Alpine::Model::ProcessMesh(aiMesh* aMesh, const aiScene* aScene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (int i = 0; i < aMesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.pos.x = aMesh->mVertices[i].x;
		vertex.pos.y = aMesh->mVertices[i].y;
		vertex.pos.z = aMesh->mVertices[i].z;

		if (aMesh->mTextureCoords[0])
		{
			vertex.texCoord.x = (float)aMesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)aMesh->mTextureCoords[0][i].y;
		}

		vertex.norm.x = aMesh->mNormals[i].x;
		vertex.norm.y = aMesh->mNormals[i].y;
		vertex.norm.z = aMesh->mNormals[i].z;
		
		vertices.push_back(vertex);
	}

	for (int i = 0; i < aMesh->mNumFaces; i++)
	{
		aiFace face = aMesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	return Mesh(vertices, indices);
}

void Alpine::Model::Draw()
{
	DX11::GetDeviceContext()->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
	DX11::GetDeviceContext()->PSSetSamplers(0, 1, myTextureSamplerState.GetAddressOf());
	for (int i = 0; i < myMeshes.size(); i++)
	{
		myMeshes[i].Draw();
	}
}

Matrix& Alpine::Model::GetTransform()
{
	return myTransform;
}
