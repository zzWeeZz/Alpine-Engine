#include "PixelShader.h"

#include <d3dcompiler.h>

bool Alpine::PixelShader::Initialize(const std::filesystem::path& filePath)
{
	auto hr = D3DReadFileToBlob(filePath.c_str(), myBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = DX11::Device()->CreatePixelShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
}

ID3D11PixelShader* Alpine::PixelShader::GetShader() const
{
	return myShader.Get();
}

ID3D10Blob* Alpine::PixelShader::GetBuffer()
{
	return myBuffer.Get();
}

void Alpine::PixelShader::Bind()
{
	DX11::Context()->PSSetShader(myShader.Get(), nullptr, 0);
}
