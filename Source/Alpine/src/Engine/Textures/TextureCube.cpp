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
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = 2048;
    texDesc.Height = 2048;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 6;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.CPUAccessFlags = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
    SMViewDesc.Format = texDesc.Format;
    SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
    SMViewDesc.TextureCube.MostDetailedMip = 0;

    D3D11_SUBRESOURCE_DATA pData[6];
    std::vector<Vector4> d[6]; // 6 images of type vector4b = 4 * unsigned char

    for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
    {
        d[cubeMapFaceIndex].resize(2048 * 2048);

        // fill with red color  
        std::fill(
            d[cubeMapFaceIndex].begin(),
            d[cubeMapFaceIndex].end(),
            Vector4(255, 255, 255, 255));

        pData[cubeMapFaceIndex].pSysMem = &d[cubeMapFaceIndex][0];// description.data;
        pData[cubeMapFaceIndex].SysMemPitch = 2048 * 4;
        pData[cubeMapFaceIndex].SysMemSlicePitch = 0;
    }

    HRESULT hr = DX11::GetDevice()->CreateTexture2D(&texDesc,
        &pData[0], &myTexture);
    assert(hr == S_OK);

    hr = DX11::GetDevice()->CreateShaderResourceView(myTexture.Get(), &SMViewDesc, &myShaderResourceView);
    assert(hr == S_OK);
}

void Alpine::TextureCube::Bind(unsigned slot)
{
	DX11::GetDeviceContext()->PSSetSamplers(slot, 1, mySamplerState.GetAddressOf());
	DX11::GetDeviceContext()->PSSetShaderResources(slot, 1, myShaderResourceView.GetAddressOf());
}

std::shared_ptr<Alpine::TextureCube> Alpine::TextureCube::Create(const std::filesystem::path& filePath)
{
	return std::make_shared<TextureCube>(filePath);
}
