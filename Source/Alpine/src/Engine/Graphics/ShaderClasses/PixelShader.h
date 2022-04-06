#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include "DX11/DX11.h"

namespace Alpine
{
	class PixelShader
	{
	public:
		bool Initialize(std::wstring aShaderPath);
		ID3D11PixelShader* GetShader() const;
		ID3D10Blob* GetBuffer() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> myShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> myBuffer;
	};
}
