#include "PixelShader.h"



bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath)
{
	auto hr = D3DReadFileToBlob(aShaderPath.c_str(), myBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = aDevice->CreatePixelShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
}

ID3D11PixelShader* PixelShader::GetShader() const
{
	return myShader.Get();
}

ID3D10Blob* PixelShader::GetBuffer() const
{
	return myBuffer.Get();
}
