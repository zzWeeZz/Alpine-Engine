#pragma once
#include "Shader.h"

namespace Alpine
{
	class ComputeShader : public Shader
	{
	public:
		bool Initialize(const std::filesystem::path& filePath) override;
		ID3D11ComputeShader* GetShader() const { return m_Shader.Get(); }
		ID3D10Blob* GetBuffer() override;
		~ComputeShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_Shader;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
	};
}