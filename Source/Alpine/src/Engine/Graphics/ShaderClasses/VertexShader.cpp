#include "VertexShader.h"

bool Alpine::VertexShader::Initialize(std::wstring aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements)
{
	
	auto hr = D3DReadFileToBlob(aShaderPath.c_str(), myBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = DX11::GetDevice()->CreateVertexShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = DX11::GetDevice()->CreateInputLayout(aDesc, aElements, myBuffer->GetBufferPointer(), myBuffer->GetBufferSize(), &myInputLayout);

	return true;
}

ID3D11VertexShader* Alpine::VertexShader::GetShader()
{
	return myShader.Get();
}

ID3D10Blob* Alpine::VertexShader::GetBuffer()
{
	return myBuffer.Get();
}

ID3D11InputLayout* Alpine::VertexShader::GetInputLayout()
{
	return myInputLayout.Get();
}
