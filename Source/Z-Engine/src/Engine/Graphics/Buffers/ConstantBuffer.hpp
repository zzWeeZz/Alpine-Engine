#include <d3d11.h>
#include <wrl.h>
#include <memory>

template<class T>
class ConstantBuffer
{
public:
	void CreateBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void UpdateBuffer(T* data);
	void SetData(const T* data, uint32_t size);

	void Bind();
private:
	ID3D11Buffer* myBuffer;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	UINT myBufferSize;
};

template<class T>
inline void ConstantBuffer<T>::CreateBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	myDevice = device;
	myContext = deviceContext;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(T);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	myDevice->CreateBuffer(&bufferDesc, NULL, &myBuffer);
}

template<class T>
inline void ConstantBuffer<T>::UpdateBuffer(T* data)
{
	myContext->UpdateSubresource(myBuffer, 0, NULL, data, 0, 0);
}

template <class T>
void ConstantBuffer<T>::SetData(const T* data, uint32_t size)
{
	D3D11_MAPPED_SUBRESOURCE subResource = {};
	myContext->Map(myBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy(subResource.pData, data, size);
	myContext->Unmap(myBuffer, 0);
}

template<class T>
inline void ConstantBuffer<T>::Bind()
{
	myContext->VSSetConstantBuffers(0, 1, &myBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myBuffer);
}
