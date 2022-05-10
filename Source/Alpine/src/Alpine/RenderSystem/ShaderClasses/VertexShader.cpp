#include "VertexShader.h"
#include <d3dcompiler.h>




Alpine::VertexShader::VertexShader(const std::filesystem::path& aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements)
{
	AssertIfFailed(D3DReadFileToBlob(aShaderPath.c_str(), myBuffer.GetAddressOf()));
	AssertIfFailed(DX11::Device()->CreateVertexShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf()));
	AssertIfFailed(DX11::Device()->CreateInputLayout(aDesc, aElements, myBuffer->GetBufferPointer(), myBuffer->GetBufferSize(), &myInputLayout));
}

void Alpine::VertexShader::Bind()
{
	DX11::Context()->IASetInputLayout(myInputLayout.Get());
	DX11::Context()->VSSetShader(m_Shader.Get(), NULL, 0);
}

Alpine::Ref<Alpine::VertexShader> Alpine::VertexShader::Create(const std::filesystem::path& aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements)
{
	return std::make_shared<VertexShader>(aShaderPath, aDesc, aElements);
}
