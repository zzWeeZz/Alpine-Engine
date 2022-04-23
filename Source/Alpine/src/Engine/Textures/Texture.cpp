#include "Texture.h"

#include <cassert>
#include <DirectXTK/WICTextureLoader.h>
#include <DX11/DX11.h>

Alpine::Texture::Texture(const std::filesystem::path& aPath)
{
	auto hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(),aPath.wstring().c_str(), &m_Resource,m_ShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = m_Resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_Texture.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void Alpine::Texture::Bind(unsigned int slot, bool useCompute)
{
	if (!useCompute)
	{
		Alpine::DX11::GetDeviceContext()->PSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf());
	}
	else
	{
		Alpine::DX11::GetDeviceContext()->CSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf());
	}
}

ComPtr<ID3D11Resource> Alpine::Texture::GetResource()
{
	return m_Resource;
}

ComPtr<ID3D11ShaderResourceView> Alpine::Texture::GetShaderResourceView()
{
	return m_ShaderResourceView;
}

std::shared_ptr<Alpine::Texture> Alpine::Texture::Create(const std::filesystem::path& aPath)
{
	return std::make_shared<Texture>(Texture(aPath));
}
