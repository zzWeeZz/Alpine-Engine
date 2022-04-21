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

		void Bind(bool compute = false, uint8_t slot = 0);
	private:
		ID3D11Buffer* myBuffer;
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

		DX11::GetDevice()->CreateBuffer(&bufferDesc, NULL, &myBuffer);
	}

	template <class T>
	void ConstantBuffer<T>::SetData(const T* data, uint32_t size)
	{

		D3D11_MAPPED_SUBRESOURCE subResource = {};
		DX11::GetDeviceContext()->Map(myBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		memcpy(subResource.pData, data, size);
		DX11::GetDeviceContext()->Unmap(myBuffer, 0);
	}

	template<class T>
	inline void ConstantBuffer<T>::Bind(bool compute, uint8_t slot)
	{
		if (!compute)
		{
			DX11::GetDeviceContext()->VSSetConstantBuffers(myBindSlot, 1, &myBuffer);
			DX11::GetDeviceContext()->PSSetConstantBuffers(myBindSlot, 1, &myBuffer);
		}
		else
		{
			DX11::GetDeviceContext()->CSSetConstantBuffers(slot, 1, &myBuffer);
		}
	}
}