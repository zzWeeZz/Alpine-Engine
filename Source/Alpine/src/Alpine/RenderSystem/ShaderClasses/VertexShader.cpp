#include "VertexShader.h"
#include <d3dcompiler.h>


bool Alpine::VertexShader::m_HasSetInputLayout;
Alpine::VertexShader::VertexShader(const std::filesystem::path& aShaderPath)
{
	AssertIfFailed(D3DReadFileToBlob(aShaderPath.c_str(), m_Buffer.GetAddressOf()));
	AssertIfFailed(DX11::Device()->CreateVertexShader(m_Buffer.Get()->GetBufferPointer(), m_Buffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf()));
	if (!VertexShader::m_HasSetInputLayout)
	{
		InputLayout layoutnew =
		{
			{"POSITION", Value::FLOAT3},
			{"TEXCOORD", Value::FLOAT2},
			{"NORMAL", Value::FLOAT3},
			{"TANGENT", Value::FLOAT3},
			{"BITANGENT", Value::FLOAT3},
			{"SHADOWPOS", Value::FLOAT3}
		};
		AssertIfFailed(DX11::Device()->CreateInputLayout(layoutnew.inputElements.data(), layoutnew.inputElements.size(), m_Buffer->GetBufferPointer(), m_Buffer->GetBufferSize(), &m_InputLayout));
		DX11::Context()->IASetInputLayout(m_InputLayout.Get());
		VertexShader::m_HasSetInputLayout = true;
	}
}

void Alpine::VertexShader::Bind()
{
	DX11::Context()->VSSetShader(m_Shader.Get(), NULL, 0);
}

Alpine::Ref<Alpine::VertexShader> Alpine::VertexShader::Create(const std::filesystem::path& aShaderPath)
{
	return std::make_shared<VertexShader>(aShaderPath);
}