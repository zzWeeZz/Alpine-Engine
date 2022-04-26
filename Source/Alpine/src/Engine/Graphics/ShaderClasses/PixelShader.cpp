#include "PixelShader.h"

#include <d3dcompiler.h>


Alpine::PixelShader::PixelShader(const std::filesystem::path& filePath)
{
	AssertIfFailed(D3DReadFileToBlob(filePath.c_str(), m_Buffer.GetAddressOf()));
	AssertIfFailed(DX11::Device()->CreatePixelShader(m_Buffer.Get()->GetBufferPointer(), m_Buffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf()));	
}

void Alpine::PixelShader::Bind()
{
	DX11::Context()->PSSetShader(m_Shader.Get(), nullptr, 0);
}

void Alpine::PixelShader::Unbind()
{
	DX11::Context()->PSSetShader(nullptr, nullptr, 0);
}

Alpine::Ref<Alpine::PixelShader> Alpine::PixelShader::Create(const std::filesystem::path& filePath)
{
	return std::make_shared<PixelShader>(filePath);
}
