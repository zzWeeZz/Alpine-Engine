#include "TextureCube.h"

#include "DX11/DX11.h"
#include <cassert>
#include <DirectXTK/WICTextureLoader.h>
#include <spdlog/spdlog.h>
#include <DirectXTK/SimpleMath.h>
Alpine::TextureCube::TextureCube(const std::filesystem::path& filePath)
{
	std::wstring path = filePath.wstring().c_str();
	path += L"/negz.jpg";
	auto hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::Device().Get(), path.c_str(), &m_TextureViews[0], m_ShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/posz.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::Device().Get(), path.c_str(), &m_TextureViews[1], m_ShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/posy.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::Device().Get(), path.c_str(), &m_TextureViews[2], m_ShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/negy.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::Device().Get(), path.c_str(), &m_TextureViews[3], m_ShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/posx.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::Device().Get(), path.c_str(), &m_TextureViews[4], m_ShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	path = filePath.wstring().c_str();
	path += L"/negx.jpg";
	hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::Device().Get(), path.c_str(), &m_TextureViews[5], m_ShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));

	D3D11_TEXTURE2D_DESC texElementDesc;
	((ID3D11Texture2D*)m_TextureViews[0])->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc = {};
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	if (FAILED(DX11::Device()->CreateTexture2D(&texArrayDesc, 0, &m_Texture)))
	{
		spdlog::error("Failed to create texture array");
		return;
	}
	// Copy individual texture elements into texture array.
	ID3D11DeviceContext* pd3dContext;
	DX11::Device()->GetImmediateContext(&pd3dContext);
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

			pd3dContext->CopySubresourceRegion(m_Texture, D3D11CalcSubresource(mipLevel, x, texArrayDesc.MipLevels), 0, 0, 0, m_TextureViews[x], mipLevel, &sourceRegion);
		}
	}
	D3D11_TEXTURE2D_DESC desc;
	m_Texture->GetDesc(&desc);
	m_Levels = desc.MipLevels;
	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

	if (FAILED(DX11::Device()->CreateShaderResourceView(m_Texture, &viewDesc, &m_ShaderResourceView)))
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

	DX11::Device()->CreateSamplerState(&samplerDesc, &m_TextureSamplerState);
}

Alpine::TextureCube::TextureCube(UINT width, UINT height, DXGI_FORMAT format, UINT levels)
{
	D3D11_TEXTURE2D_DESC texElementDesc;
	texElementDesc.Width = width;
	texElementDesc.Height = height;
	texElementDesc.MipLevels = levels;
	texElementDesc.ArraySize = 6;
	texElementDesc.Format = format;
	texElementDesc.SampleDesc.Count = 1;
	texElementDesc.SampleDesc.Quality = 0;
	texElementDesc.Usage = D3D11_USAGE_DEFAULT;
	texElementDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texElementDesc.CPUAccessFlags = 0;
	texElementDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	if (levels == 0)
	{
		texElementDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		texElementDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	m_Levels = levels;
	if (FAILED(DX11::Device()->CreateTexture2D(&texElementDesc, 0, &m_Texture)))
	{
		spdlog::error("Failed to create texture array");
		return;
	}
	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texElementDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = -1;

	if (FAILED(DX11::Device()->CreateShaderResourceView(m_Texture, &viewDesc, &m_ShaderResourceView)))
	{
		spdlog::error("Failed to create shader resource view");
		return;
	}
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX11::Device()->CreateSamplerState(&samplerDesc, &m_TextureSamplerState);
}

void Alpine::TextureCube::Bind(unsigned int slot, bool forCompute)
{
	if (!forCompute)
	{
		DX11::Context()->PSSetSamplers(slot, 1, &m_TextureSamplerState);
		DX11::Context()->PSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf());
	}
	else
	{
		DX11::Context()->CSSetSamplers(slot, 1, &m_TextureSamplerState);
		DX11::Context()->CSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf());
	}
}

ID3D11Resource* Alpine::TextureCube::GetTextures()
{
	return *m_TextureViews.data();
}

void Alpine::TextureCube::CreateUAV(UINT levels)
{
	D3D11_TEXTURE2D_DESC desc;
	m_Texture->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = desc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.MipSlice = levels;
	uavDesc.Texture2DArray.FirstArraySlice = 0;
	uavDesc.Texture2DArray.ArraySize = desc.ArraySize;

	auto hr = DX11::Device()->CreateUnorderedAccessView(m_Texture, &uavDesc, &m_UAV);
	assert(SUCCEEDED(hr));
}

unsigned int Alpine::TextureCube::GetLevels()
{
	return m_Levels;
}

std::shared_ptr<Alpine::TextureCube> Alpine::TextureCube::Create(const std::filesystem::path& filePath)
{
	return std::make_shared<TextureCube>(filePath);
}

std::shared_ptr<Alpine::TextureCube> Alpine::TextureCube::Create(UINT width, UINT height, DXGI_FORMAT format, UINT levels)
{
	return std::make_shared<TextureCube>(width, height, format, levels);
}
