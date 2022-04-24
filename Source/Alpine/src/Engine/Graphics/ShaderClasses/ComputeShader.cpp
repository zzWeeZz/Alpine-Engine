#include "ComputeShader.h"

#include "DX11/DX11.h"
#include <cassert>
#include <d3dcompiler.h>


bool Alpine::ComputeShader::Initialize(const std::filesystem::path& filePath)
{
	auto hr = D3DReadFileToBlob(filePath.c_str(), myBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = DX11::Device()->CreateComputeShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
	assert(SUCCEEDED(hr));
	return true;
}

ID3D10Blob* Alpine::ComputeShader::GetBuffer()
{
	return myBuffer.Get();
}


Alpine::ComputeShader::~ComputeShader()
{
	m_Shader.Reset();
	myBuffer.Reset();
}