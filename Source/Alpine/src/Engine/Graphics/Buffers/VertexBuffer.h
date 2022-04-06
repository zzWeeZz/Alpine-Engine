#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "DX11/DX11.h"

namespace Alpine
{
	template<class T>
	class VertexBuffer
	{
	public:
		VertexBuffer() {};

		VertexBuffer(const VertexBuffer<T>& aBuffer);

		VertexBuffer<T>& operator=(const VertexBuffer<T>& a);

		ID3D11Buffer* Get() const { return myBuffer.Get(); }

		ID3D11Buffer* const* GetAddressOf() const { return myBuffer.GetAddressOf(); }

		UINT GetBufferSize() const { return myBufferSize; }

		const UINT Stride() const { return *myStride.get(); }

		const UINT* StridePtr() const { return myStride.get(); }

		HRESULT Initialize(T* aData, UINT aVertexCount);

	private:

		Microsoft::WRL::ComPtr<ID3D11Buffer> myBuffer;
		std::shared_ptr<UINT> myStride;
		UINT myBufferSize = 0;
	};
}
template<class T>
inline Alpine::VertexBuffer<T>& Alpine::VertexBuffer<T>::operator=(const VertexBuffer<T>& a)
{
	this->myBuffer = a.myBuffer;
	this->myBufferSize = a.myBufferSize;
	this->myStride = a.myStride;
	return *this;
}

template<class T>
inline HRESULT Alpine::VertexBuffer<T>::Initialize(T* aData, UINT aVertexCount)
{
	myBufferSize = aVertexCount;
	myStride = std::make_shared<UINT>(sizeof(T));

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(T) * aVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = aData;
	auto hr = DX11::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, myBuffer.GetAddressOf());
	return hr;
}