#pragma once
#include "Mesh.h"


class Model
{
public:
	void Initialize(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& aContext, const Microsoft::WRL::ComPtr<ID3D11Device>& aDevice);
	void SetModel(std::wstring aPath, std::wstring aTexturePath = L"Textures/Default.png");
	void Draw(ID3D11RasterizerState* state);

	ToolBox::Math::Transform& GetTransform();

private:
	void PrepareForRender();
	DWORD myIndex[36];
	bool LoadModel(const std::string& aFilePath);
	void ProcessNode(aiNode* aNode, const aiScene* aScene);
	Mesh ProcessMesh(aiMesh* aMesh, const aiScene* aScene);

	std::vector<Mesh> myMeshes;

	ToolBox::Math::Transform myTransform;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;
	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
	Microsoft::WRL::ComPtr< ID3D11SamplerState> myTextureSamplerState;
};
