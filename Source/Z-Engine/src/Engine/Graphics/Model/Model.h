#pragma once
#include "Mesh.h"
#include <string>
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;

class Model
{
public:
	void Initialize(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& aContext, const Microsoft::WRL::ComPtr<ID3D11Device>& aDevice);
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

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;
	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
	Microsoft::WRL::ComPtr< ID3D11SamplerState> myTextureSamplerState;
};
