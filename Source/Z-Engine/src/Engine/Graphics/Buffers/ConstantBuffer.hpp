#include <d3d11.h>
#include <wrl.h>
#include <memory>

template<class T>
class ConstantBuffer
{
public:
	void CreateBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void UpdateBuffer(T* data);
	void UpdateBuffer(ID3D11DeviceContext* deviceContext, T* data, UINT bufferSize);
	void UpdateBuffer(ID3D11DeviceContext* deviceContext, T* data, UINT bufferSize, UINT offset);

	void Bind();
	void Bind(ID3D11DeviceContext* deviceContext, UINT slot);
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

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
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

template<class T>
inline void ConstantBuffer<T>::UpdateBuffer(ID3D11DeviceContext* deviceContext, T* data, UINT bufferSize)
{
	UpdateBuffer(deviceContext, data, bufferSize, 0);
}

template<class T>
inline void ConstantBuffer<T>::UpdateBuffer(ID3D11DeviceContext* deviceContext, T* data, UINT bufferSize, UINT offset)
{
	myContext->UpdateSubresource(myBuffer, 0, NULL, data, 0, 0);
}

template<class T>
inline void ConstantBuffer<T>::Bind()
{
	myContext->VSSetConstantBuffers(0, 1, &myBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myBuffer);
}

template<class T>
inline void ConstantBuffer<T>::Bind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	deviceContext->VSSetConstantBuffers(slot, 1, &myBuffer);
	deviceContext->PSSetConstantBuffers(slot, 1, &myBuffer);
}
