#pragma once
#include "shader.h"
#include "Alpine/DX11/Utilities.h"
#include "Alpine/DX11/DX11.h"

namespace Alpine
{
	enum class Value
	{
		FLOAT4 = 2,
		UINT4 = 3,
		FLOAT3 = 6,
		UINT3 = 7,
		FLOAT2 = 16,
		UINT2 = 17,
		FLOAT = 41,
		UINT = 42,
		FLOAT4X4 = FLOAT4,
		UINT4X4 = UINT4,
		FLOAT3X3 = FLOAT3,
		UINT3X3 = UINT3,
	};
	struct InputElementSpec
	{
		LPCSTR name;
		Value value;
		uint32_t offset = 0;
	};
	struct InputLayout
	{
		InputLayout(std::initializer_list<InputElementSpec> elements)
		{
			for (auto& element : elements)
			{
				inputElements.emplace_back(element.name, element.offset, static_cast<DXGI_FORMAT>(element.value), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
			}
		}
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements{};
	};

	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::filesystem::path& aShaderPath);
		
		void Bind() override;

		static Ref<VertexShader> Create(const std::filesystem::path& aShaderPath);

	private:
		ComPtr<ID3D11VertexShader> m_Shader;
		ComPtr<ID3D10Blob> m_Buffer;
		ComPtr<ID3D11InputLayout> m_InputLayout;
		static bool m_HasSetInputLayout;
	};
}
