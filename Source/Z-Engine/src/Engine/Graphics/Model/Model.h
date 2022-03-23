#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXTK/WICTextureLoader.h>
#include <string>
#include "../../ToolBox/src/ToolBox/Math/Transform.h"
#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexBuffer.h"

#include "ModelData.hpp"


class Model
{
public:
	void Initialize(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& aContext, const Microsoft::WRL::ComPtr<ID3D11Device>& aDevice);
	void SetModel(std::wstring aPath, std::wstring aTexturePath = L"Textures/Default.png");
	void Draw();

	ToolBox::Math::Transform& GetTransform();

private:
	void PrepareForRender();
	DWORD myIndex[36];

	ToolBox::Math::Transform myTransform;

	VertexBuffer<Vertex> myVertexBuffer;
	IndexBuffer myIndexBuffer;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;
	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
	Microsoft::WRL::ComPtr< ID3D11SamplerState> myTextureSamplerState;
};
