#pragma once
#include "shader.h"
#include "Alpine/DX11/Utilities.h"
#include "Alpine/DX11/DX11.h"

namespace Alpine
{
	enum class Value
	{
		FLOAT = 4,
		VEC2 = 8,
		VEC3 = 12,
		VEC4 = 16,
	};

	struct InputElementSpec
	{
		std::string name;
		Value value;
		InputElementSpec(Value size, std::string identifier)
		{
			value = size;
			name = identifier;
		}
		
	};

	struct VertexLayoutSpecs
	{
		VertexLayoutSpecs(std::vector<InputElementSpec>& specs)
		{
			theSpecs = specs;
		}
		std::vector<InputElementSpec> theSpecs;
	};
	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::filesystem::path& aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements);
		
		void Bind() override;

		static Ref<VertexShader> Create(const std::filesystem::path& aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT aElements);

	private:
		ComPtr<ID3D11VertexShader> m_Shader;
		ComPtr<ID3D10Blob> myBuffer;
		ComPtr<ID3D11InputLayout> myInputLayout;
	};
}
