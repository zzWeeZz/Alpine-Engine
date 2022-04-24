#pragma once
#include "shader.h"
#include "DX11/Utilities.h"
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
		ComPtr<ID3D11VertexShader> myShader;
		ComPtr<ID3D10Blob> myBuffer;
		ComPtr<ID3D11InputLayout> myInputLayout;
	};
}
