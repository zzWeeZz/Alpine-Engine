#include "IndexBuffer.h"
#include "DX11/DX11.h"

HRESULT Alpine::IndexBuffer::Initalize(DWORD* aData, UINT aNumberOfIndices)
{
	myBufferSize = aNumberOfIndices;

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * aNumberOfIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = aData;
	auto hr = DX11::GetDevice()->CreateBuffer(&indexBufferDesc, &iinitData, &myIndexsBuffer);
	return hr;
}

ID3D11Buffer* Alpine::IndexBuffer::Get() const
{
	return myIndexsBuffer.Get();
}

ID3D11Buffer* const* Alpine::IndexBuffer::GetAddressOf() const
{
	return myIndexsBuffer.GetAddressOf();
}

UINT Alpine::IndexBuffer::BufferSize() const
{
	return myBufferSize;
}
