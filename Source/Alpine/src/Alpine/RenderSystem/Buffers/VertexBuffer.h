#pragma once
#include <d3d11.h>
#include "Alpine/DX11/Utilities.h"
#include "Alpine/DX11/DX11.h"

namespace Alpine
{
	template<class T>
	class VertexBuffer
	{
	public:
		VertexBuffer() {};

		VertexBuffer(const VertexBuffer<T>& aBuffer);

		VertexBuffer<T>& operator=(const VertexBuffer<T>& a);

		ID3D11Buffer* Get() const { return m_Buffer.Get(); }

		ID3D11Buffer* const* GetAddressOf() const { return m_Buffer.GetAddressOf(); }

		UINT GetBufferSize() const { return m_BufferSize; }

		const UINT Stride() const { return *m_Stride.get(); }

		const UINT* StridePtr() const { return m_Stride.get(); }

		HRESULT Initialize(T* aData, UINT aVertexCount);

	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		std::shared_ptr<UINT> m_Stride;
		UINT m_BufferSize = 0;
	};
}

template<class T>
inline Alpine::VertexBuffer<T>& Alpine::VertexBuffer<T>::operator=(const VertexBuffer<T>& a)
{
	this->m_Buffer = a.m_Buffer;
	this->m_BufferSize = a.m_BufferSize;
	this->m_Stride = a.m_Stride;
	return *this;
}

template<class T>
inline HRESULT Alpine::VertexBuffer<T>::Initialize(T* aData, UINT aVertexCount)
{
	m_BufferSize = aVertexCount;
	m_Stride = std::make_shared<UINT>(sizeof(T));

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(T) * aVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = aData;
	auto hr = DX11::Device()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_Buffer.GetAddressOf());
	return hr;
}