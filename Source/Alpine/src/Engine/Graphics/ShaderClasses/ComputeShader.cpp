#include "ComputeShader.h"

#include "DX11/DX11.h"
#include <cassert>


bool Alpine::ComputeShader::Initialize(const std::filesystem::path& filePath)
{
	auto hr = D3DReadFileToBlob(filePath.c_str(), myBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = DX11::GetDevice()->CreateComputeShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf());
	assert(SUCCEEDED(hr));
	return true;
}

ID3D10Blob* Alpine::ComputeShader::GetBuffer()
{
	return myBuffer.Get();
}

Alpine::ComputeShader::~ComputeShader()
{
	myShader.Reset();
	myBuffer.Reset();
}