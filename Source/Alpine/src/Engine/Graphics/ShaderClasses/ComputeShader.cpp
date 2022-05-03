#include "ComputeShader.h"

#include "Engine/DX11/DX11.h"
#include <cassert>
#include <d3dcompiler.h>


Alpine::ComputeShader::ComputeShader(const std::filesystem::path& filePath)
{
	auto hr = D3DReadFileToBlob(filePath.c_str(), m_Buffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = DX11::Device()->CreateComputeShader(m_Buffer.Get()->GetBufferPointer(), m_Buffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
	assert(SUCCEEDED(hr));
}

Alpine::ComputeShader::~ComputeShader()
{
	m_Shader.Reset();
}

void Alpine::ComputeShader::Bind()
{
}

Alpine::Ref<Alpine::ComputeShader> Alpine::ComputeShader::Create(const std::filesystem::path& filePath)
{
	return CreateRef<ComputeShader>(filePath);
}
