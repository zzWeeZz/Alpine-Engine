#include "PixelShader.h"



bool Alpine::PixelShader::Initialize(std::wstring aShaderPath)
{
	auto hr = D3DReadFileToBlob(aShaderPath.c_str(), myBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = DX11::GetDevice()->CreatePixelShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
}

ID3D11PixelShader* Alpine::PixelShader::GetShader() const
{
	return myShader.Get();
}

ID3D10Blob* Alpine::PixelShader::GetBuffer() const
{
	return myBuffer.Get();
}
