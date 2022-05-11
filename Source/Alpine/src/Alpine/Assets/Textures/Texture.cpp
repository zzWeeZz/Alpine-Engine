#include "Texture.h"

#include <cassert>
#include <DirectXTK/WICTextureLoader.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <Alpine/DX11/DX11.h>

Alpine::Texture::Texture(const std::filesystem::path& aPath, bool isSRGB)
{
	if(stbi_is_hdr(aPath.string().c_str()))
	{
		float* pixels = stbi_loadf(aPath.string().c_str(), &m_Width, &m_Height, &m_Channels, 4);
		if(pixels)
		{
			m_Pixels.reset(reinterpret_cast<unsigned char*>(pixels));
			m_IsHDR = true;
			CreateTextureFromHdrData();
		}
	}
	else
	{
		m_IsHDR = false;
		if (isSRGB)
		{
			float* pixels = stbi_loadf(aPath.string().c_str(), &m_Width, &m_Height, &m_Channels, 4);
			if (pixels)
			{
				m_Pixels.reset(reinterpret_cast<unsigned char*>(pixels));
				CreateTextureFromImageData(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
			}
		}
		else
		{
			float* pixels = stbi_loadf(aPath.string().c_str(), &m_Width, &m_Height, &m_Channels, 4);
			if (pixels)
			{
				m_Pixels.reset(reinterpret_cast<unsigned char*>(pixels));
				CreateTextureFromImageData(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
			}
			/*AssertIfFailed(DirectX::CreateWICTextureFromFileEx(Alpine::DX11::Device().Get(),
				aPath.wstring().c_str(),
				0,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS,
				0,
				D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_GENERATE_MIPS,
				DirectX::WIC_LOADER_DEFAULT,
				m_Resource.GetAddressOf(),
				m_ShaderResourceView.GetAddressOf()));
			AssertIfFailed(m_Resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_Texture.GetAddressOf())));
			DX11::Context()->GenerateMips(m_ShaderResourceView.Get());*/

		}
			
		
		// gen mip maps
	}
}

Alpine::Texture::Texture(UINT width, UINT height, DXGI_FORMAT format, UINT levels)
{
	m_Height = height;
	m_Width = width;
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
	//gen mip maps
	if (levels == 0)
	{
		DX11::Context()->GenerateMips(m_ShaderResourceView.Get());
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

void Alpine::Texture::CreateTextureFromImageData(DXGI_FORMAT format)
{
	Ref<Texture> text = Create(m_Width, m_Height, format);

	DX11::Context()->UpdateSubresource(text->m_Texture.Get(), 0, 0, m_Pixels.get(), m_Width * 4 * sizeof(float), 0);
	if (text->m_Texture)
	{
		DX11::Context()->GenerateMips(text->m_ShaderResourceView.Get());
	}
	m_Texture = text->m_Texture;
	m_ShaderResourceView = text->m_ShaderResourceView;
}

void Alpine::Texture::CreateTextureFromHdrData()
{
	Ref<Texture> text = Create(m_Width, m_Height, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	DX11::Context()->UpdateSubresource(text->m_Texture.Get(), 0, 0, m_Pixels.get(), m_Width * 4 * sizeof(float), 0);
	if (text->m_Texture)
	{
		DX11::Context()->GenerateMips(text->m_ShaderResourceView.Get());
	}
	m_Texture = text->m_Texture;
	m_ShaderResourceView = text->m_ShaderResourceView;
}


Alpine::Ref<Alpine::Texture> Alpine::Texture::Create(const std::filesystem::path& aPath, bool isSRGB)
{
	return std::make_shared<Texture>(aPath, isSRGB);

}

Alpine::Ref<Alpine::Texture> Alpine::Texture::Create(UINT width, UINT height, DXGI_FORMAT format, UINT levels)
{
	return std::make_shared<Texture>(width, height, format, levels);
}

