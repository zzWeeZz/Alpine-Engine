#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "DX11/DX11.h"

namespace Alpine
{
	template<class T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer(uint32_t slot);
		void Create();
		void SetData(const T* data, uint32_t size);
		ID3D11Buffer* GetBuffer() { return m_Buffer; }
		void Bind(bool compute = false, uint8_t slot = 0);
	private:
		ID3D11Buffer* m_Buffer;
		UINT myBufferSize;
		uint32_t myBindSlot;
	};

	template<class T>
	ConstantBuffer<T>::ConstantBuffer(uint32_t slot)
	{
		myBindSlot = slot;
	}

	template<class T>
	void ConstantBuffer<T>::Create()
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		DX11::GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_Buffer);
	}

	template <class T>
	void ConstantBuffer<T>::SetData(const T* data, uint32_t size)
	{

		D3D11_MAPPED_SUBRESOURCE subResource = {};
		DX11::GetDeviceContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		memcpy(subResource.pData, data, size);
		DX11::GetDeviceContext()->Unmap(m_Buffer, 0);
	}

	template<class T>
	inline void ConstantBuffer<T>::Bind(bool compute, uint8_t slot)
	{
		if (!compute)
		{
			DX11::GetDeviceContext()->VSSetConstantBuffers(myBindSlot, 1, &m_Buffer);
			DX11::GetDeviceContext()->PSSetConstantBuffers(myBindSlot, 1, &m_Buffer);
		}
		else
		{
			DX11::GetDeviceContext()->CSSetConstantBuffers(slot, 1, &m_Buffer);
		}
	}
}