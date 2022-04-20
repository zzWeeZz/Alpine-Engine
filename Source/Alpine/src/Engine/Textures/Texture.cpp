#include "Texture.h"

#include <cassert>
#include <DirectXTK/WICTextureLoader.h>
#include <DX11/DX11.h>

Alpine::Texture::Texture(const std::filesystem::path& aPath)
{
	auto hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(),aPath.wstring().c_str(), &myResource,myShaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = myResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(myTexture.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void Alpine::Texture::Bind(unsigned int slot)
{
	DX11::GetDeviceContext()->PSSetShaderResources(slot, 1, myShaderResourceView.GetAddressOf());
	DX11::GetDeviceContext()->CSSetShaderResources(slot, 1, myShaderResourceView.GetAddressOf());
}

ComPtr<ID3D11Resource> Alpine::Texture::GetResource()
{
	return myResource;
}

ComPtr<ID3D11ShaderResourceView> Alpine::Texture::GetShaderResourceView()
{
	return myShaderResourceView;
}

std::shared_ptr<Alpine::Texture> Alpine::Texture::Create(const std::filesystem::path& aPath)
{
	return std::make_shared<Texture>(Texture(aPath));
}
