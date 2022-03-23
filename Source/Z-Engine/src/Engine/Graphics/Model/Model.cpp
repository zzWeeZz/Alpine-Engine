#include "Model.h"
#include <iostream>


void Model::Initialize(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& aContext, const Microsoft::WRL::ComPtr<ID3D11Device>& aDevice)
{
	myContext = aContext;
	myDevice = aDevice;
}

void Model::SetModel(std::wstring aPath, std::wstring aTexturePath)
{
	auto hr = DirectX::CreateWICTextureFromFile(myDevice.Get(), aTexturePath.c_str(), nullptr, &myTexture);
	if (FAILED(hr))
	{
		std::cout << "ERROR TEXTURE DIDNT LOAD" << std::endl;
		DirectX::CreateWICTextureFromFile(myDevice.Get(), L"Textures/Default.png", nullptr, &myTexture);
	}
	if (aPath == L"Cube")
	{
		Vertex v[] =
		{
			// Front Face
			Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
			Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
			Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
			Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

			// Back Face
			Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
			Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
			Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
			Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

			// Top Face
			Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
			Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
			Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
			Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

			// Bottom Face
			Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
			Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
			Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
			Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

			// Left Face
			Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
			Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
			Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
			Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

			// Right Face
			Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
			Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
			Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
			Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
		};

		myVertexBuffer.Initialize(myDevice.Get(), v, ARRAYSIZE(v));

		DWORD index[] =
		{
			// Front Face
			0,  1,  2,
			0,  2,  3,

			// Back Face
			4,  5,  6,
			4,  6,  7,

			// Top Face
			8,  9, 10,
			8, 10, 11,

			// Bottom Face
			12, 13, 14,
			12, 14, 15,

			// Left Face
			16, 17, 18,
			16, 18, 19,

			// Right Face
			20, 21, 22,
			20, 22, 23
		};
		myIndexBuffer.Initalize(myDevice.Get(), index, ARRAYSIZE(index));
	}
	else
	{
		
	}

	PrepareForRender();
}

void Model::PrepareForRender()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	myDevice->CreateSamplerState(&samplerDesc, &myTextureSamplerState);
	UINT offset = 0;
	myContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.StridePtr(), &offset);
	myContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

}

void Model::Draw()
{
	myContext->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
	myContext->PSSetSamplers(0, 1, myTextureSamplerState.GetAddressOf());
	myContext->DrawIndexed(myIndexBuffer.BufferSize(), 0, 0);

}
