#include "Model.h"
#include <iostream>


void Model::Initialize(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& aContext, const Microsoft::WRL::ComPtr<ID3D11Device>& aDevice)
{
	myContext = aContext;
	myDevice = aDevice;
	myTransform.SetSize({ 1, 1, 1 });
}

void Model::SetModel(std::wstring aPath, std::wstring aTexturePath)
{
	auto hr = DirectX::CreateWICTextureFromFile(myDevice.Get(), aTexturePath.c_str(), nullptr, &myTexture);
	if (FAILED(hr))
	{
		std::cout << "ERROR TEXTURE DIDNT LOAD" << std::endl;
		DirectX::CreateWICTextureFromFile(myDevice.Get(), L"Textures/MissingTexture.png", nullptr, &myTexture);
	}
	if (aPath == L"Cube")
	{
		
	}
	else
	{
		LoadModel("Model/helicopter.fbx");
	}

	PrepareForRender();
}

void Model::PrepareForRender()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	myDevice->CreateSamplerState(&samplerDesc, &myTextureSamplerState);
}

bool Model::LoadModel(const std::string& aFilePath)
{
	Assimp::Importer importer;
	
	const aiScene* pScene = importer.ReadFile(aFilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
	{
		return false;
	}
	ProcessNode(pScene->mRootNode, pScene);
	return true;
}

void Model::ProcessNode(aiNode* aNode, const aiScene* aScene)
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

Mesh Model::ProcessMesh(aiMesh* aMesh, const aiScene* aScene)
{
	std::vector<Vertex> vertices;
	std::vector <DWORD> indices;

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
	return Mesh(myDevice.Get(), myContext.Get(), vertices, indices);
}

void Model::Draw(ID3D11RasterizerState* state) 
{
	myContext->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
	myContext->PSSetSamplers(0, 1, myTextureSamplerState.GetAddressOf());
	for (int i = 0; i < myMeshes.size(); i++)
	{
		myMeshes[i].Draw(state);
	}
}

ToolBox::Math::Transform& Model::GetTransform()
{
	return myTransform;
}
