#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>

namespace Alpine
{
	class IndexBuffer
	{
	public:
		HRESULT Initalize(DWORD* aData, UINT aNumberOfIndices);

		ID3D11Buffer* Get() const;
		ID3D11Buffer* const* GetAddressOf() const;
		UINT BufferSize() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> myIndexsBuffer;
		UINT myBufferSize = 0;
	};
}