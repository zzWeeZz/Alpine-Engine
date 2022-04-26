#pragma once
#include "shader.h"
#include "DX11/Utilities.h"
#include "DX11/DX11.h"

namespace Alpine
{
	struct InputElementSpecs
	{
		std::string name;
		unsigned int semanticIndex;
		DXGI_FORMAT format;
		unsigned int inputSlot;
		unsigned int alignedByteOffset;
		D3D11_INPUT_CLASSIFICATION inputSlotClass;
		unsigned int instanceDataStepRate;
	};
	class VertexShader
	{
	public:
		VertexShader(const std::filesystem::path& aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements);
		
		void Bind();

		static Ref<VertexShader> Create(const std::filesystem::path& aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements);

	private:
		ComPtr<ID3D11VertexShader> m_Shader;
		ComPtr<ID3D10Blob> myBuffer;
		ComPtr<ID3D11InputLayout> myInputLayout;
	};
}
