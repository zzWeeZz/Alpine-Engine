#include "Texture.h"

#include <cassert>
#include <DirectXTK/WICTextureLoader.h>
#include <DX11/DX11.h>

Alpine::Texture::Texture(const std::filesystem::path& aPath)
{
	ID3D11Resource* resource;
	auto hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(),aPath.wstring().c_str(), &resource,myShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(myTexture.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void Alpine::Texture::Bind(unsigned slot)
{
	DX11::GetDeviceContext()->PSSetShaderResources(slot, 1, myShaderResourceView.GetAddressOf());
}

std::shared_ptr<Alpine::Texture> Alpine::Texture::Create(const std::filesystem::path& aPath)
{
	return std::make_shared<Texture>(Texture(aPath));
}