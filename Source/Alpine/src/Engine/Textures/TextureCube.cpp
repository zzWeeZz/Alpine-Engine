#include "TextureCube.h"

#include "DX11/DX11.h"
#include <cassert>
#include <DirectXTK/WICTextureLoader.h>
#include <spdlog/spdlog.h>

Alpine::TextureCube::TextureCube(const std::filesystem::path& filePath)
{
	//ID3D11Resource* resource;
	//auto hr = DirectX::CreateWICTextureFromFile(Alpine::DX11::GetDevice(), filePath.wstring().c_str(), &resource, NULL);
	//if (FAILED(hr))
	//{
	//	throw std::exception("Failed to create texture from file.");
	//}
	//D3D11_TEXTURE2D_DESC dec = {};
	//dec.Width = 0;
	//dec.Height = 0;
	//dec.MipLevels = 0;
	//dec.ArraySize = 0;
	//dec.Format = DXGI_FORMAT_UNKNOWN;
	//dec.SampleDesc.Count = 1;
	//dec.SampleDesc.Quality = 0;
	//dec.Usage = D3D11_USAGE_IMMUTABLE;
	//dec.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	//DX11::GetDevice()->CreateTexture2D(&dec, nullptr, &myTexture);
	//if (FAILED(hr))
	//{
	//	throw std::exception("Failed to query texture interface.");
	//}
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	//srvDesc.TextureCube.MostDetailedMip = 0;
	//srvDesc.TextureCube.MipLevels = -1;
	//hr = Alpine::DX11::GetDevice()->CreateShaderResourceView(myTexture.Get(), &srvDesc, myShaderResourceView.GetAddressOf());
	//if (FAILED(hr))
	//{
	//	spdlog::warn("Failed to create shader resource view for texture.");
	//}
}

void Alpine::TextureCube::Bind(unsigned slot)
{
	DX11::GetDeviceContext()->PSSetShaderResources(slot, 1, myShaderResourceView.GetAddressOf());
}

std::shared_ptr<Alpine::TextureCube> Alpine::TextureCube::Create(const std::filesystem::path& filePath)
{
	return std::make_shared<TextureCube>(filePath);
}
