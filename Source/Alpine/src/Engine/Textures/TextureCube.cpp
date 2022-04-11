#include "TextureCube.h"

#include "DX11/DX11.h"
#include <cassert>
#include <DirectXTK/WICTextureLoader.h>

#include <spdlog/spdlog.h>
#include <DirectXTK/SimpleMath.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;
Alpine::TextureCube::TextureCube(const std::filesystem::path& filePath)
{
	std::wstring path = filePath.wstring().c_str();
	path += L"/negz.jpg";
	auto hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(), path.c_str(), &myTextureViews[0], myShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/posz.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(), path.c_str(), &myTextureViews[1], myShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/posy.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(), path.c_str(), &myTextureViews[2], myShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/negy.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(), path.c_str(), &myTextureViews[3], myShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/posx.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(), path.c_str(), &myTextureViews[4], myShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/negx.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(), path.c_str(), &myTextureViews[5], myShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));

	D3D11_TEXTURE2D_DESC texElementDesc;
	((ID3D11Texture2D*)myTextureViews[0])->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* texArray = 0;
	if (FAILED(DX11::GetDevice()->CreateTexture2D(&texArrayDesc, 0, &texArray)))
	{
		spdlog::error("Failed to create texture array");
		return;
	}

	// Copy individual texture elements into texture array.
	ID3D11DeviceContext* pd3dContext;
	DX11::GetDevice()->GetImmediateContext(&pd3dContext);
	D3D11_BOX sourceRegion;

	//Here i copy the mip map levels of the textures
	for (UINT x = 0; x < 6; x++)
	{
		for (UINT mipLevel = 0; mipLevel < texArrayDesc.MipLevels; mipLevel++)
		{
			sourceRegion.left = 0;
			sourceRegion.right = (texArrayDesc.Width >> mipLevel);
			sourceRegion.top = 0;
			sourceRegion.bottom = (texArrayDesc.Height >> mipLevel);
			sourceRegion.front = 0;
			sourceRegion.back = 1;

			//test for overflow
			if (sourceRegion.bottom == 0 || sourceRegion.right == 0)
				break;

			pd3dContext->CopySubresourceRegion(texArray, D3D11CalcSubresource(mipLevel, x, texArrayDesc.MipLevels), 0, 0, 0, myTextureViews[x], mipLevel, &sourceRegion);
		}
	}

	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

	if (FAILED(DX11::GetDevice()->CreateShaderResourceView(texArray, &viewDesc, &myShaderResourceView)))
	{
		spdlog::error("Failed to create shader resource view");
		return;
	}
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX11::GetDevice()->CreateSamplerState(&samplerDesc, &myTextureSamplerState);
}

void Alpine::TextureCube::Bind(unsigned slot)
{
	DX11::GetDeviceContext()->PSSetSamplers(slot, 1, &myTextureSamplerState);
	DX11::GetDeviceContext()->PSSetShaderResources(slot, 1, myShaderResourceView.GetAddressOf());
}

std::shared_ptr<Alpine::TextureCube> Alpine::TextureCube::Create(const std::filesystem::path& filePath)
{
	return std::make_shared<TextureCube>(filePath);
}
