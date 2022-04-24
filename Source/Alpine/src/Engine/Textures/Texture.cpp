#include "Texture.h"

#include <cassert>
#include <DirectXTK/WICTextureLoader.h>
#include <DX11/DX11.h>

Alpine::Texture::Texture(const std::filesystem::path& aPath)
{
	auto hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::Device().Get(),aPath.wstring().c_str(), &m_Resource,m_ShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = m_Resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_Texture.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

Alpine::Texture::Texture(UINT width, UINT height, DXGI_FORMAT format, UINT levels)
{
	D3D11_TEXTURE2D_DESC texElementDesc = {};
	texElementDesc.Width = width;
	texElementDesc.Height = height;
	texElementDesc.MipLevels = levels;
	texElementDesc.Format = format;
	texElementDesc.ArraySize = 1;
	texElementDesc.SampleDesc.Count = 1;
	texElementDesc.SampleDesc.Quality = 0;
	texElementDesc.Usage = D3D11_USAGE_DEFAULT;
	texElementDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	if (levels == 0)
	{
		texElementDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		texElementDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	if (FAILED(DX11::Device()->CreateTexture2D(&texElementDesc, 0, m_Texture.GetAddressOf())))
	{
		spdlog::error("Failed to create texture!");
		return;
	}
	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texElementDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = -1;

	if (FAILED(DX11::Device()->CreateShaderResourceView(m_Texture.Get(), &viewDesc, &m_ShaderResourceView)))
	{
		spdlog::error("Failed to create shader resource view");
		return;
	}
}

void Alpine::Texture::Bind(unsigned int slot, bool useCompute)
{
	if (!useCompute)
	{
		Alpine::DX11::Context()->PSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf());
	}
	else
	{
		Alpine::DX11::Context()->CSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf());
	}
}

void Alpine::Texture::CreateUAV(UINT levels)
{
	D3D11_TEXTURE2D_DESC desc;
	m_Texture->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = desc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = levels;


	auto hr = DX11::Device()->CreateUnorderedAccessView(m_Texture.Get(), &uavDesc, &m_UAV);
	assert(SUCCEEDED(hr));
}

ComPtr<ID3D11Resource> Alpine::Texture::GetResource()
{
	return m_Resource;
}

ComPtr<ID3D11UnorderedAccessView> Alpine::Texture::GetUnorderedAccessView()
{
	return m_UAV;
}

ComPtr<ID3D11ShaderResourceView> Alpine::Texture::GetShaderResourceView()
{
	return m_ShaderResourceView;
}

Alpine::Ref<Alpine::Texture> Alpine::Texture::Create(const std::filesystem::path& aPath)
{
	return std::make_shared<Texture>(Texture(aPath));
}

Alpine::Ref<Alpine::Texture> Alpine::Texture::Create(UINT width, UINT height, DXGI_FORMAT format, UINT levels)
{
	return std::make_shared<Texture>(width, height, format, levels);
}
