#pragma once
#include <d3d11.h>
#include "DX11/Utilities.h"
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
		ComPtr<ID3D11Buffer> m_IndexsBuffer;
		UINT m_BufferSize = 0;
	};
}