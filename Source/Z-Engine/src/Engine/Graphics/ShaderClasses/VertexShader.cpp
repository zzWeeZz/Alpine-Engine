#include "VertexShader.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements)
{
	auto hr = D3DReadFileToBlob(aShaderPath.c_str(), myBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = aDevice->CreateVertexShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = aDevice->CreateInputLayout(aDesc, aElements, myBuffer->GetBufferPointer(), myBuffer->GetBufferSize(), &myInputLayout);

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return myShader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return myBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return myInputLayout.Get();
}
