#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include "DX11/DX11.h"

namespace Alpine
{
	class VertexShader
	{
	public:
		bool Initialize(std::wstring aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements);
		ID3D11VertexShader* GetShader();
		ID3D10Blob* GetBuffer();
		ID3D11InputLayout* GetInputLayout();

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> myShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> myBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> myInputLayout;
	};
}
