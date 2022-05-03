#pragma once
#include "Shader.h"

namespace Alpine
{
	class ComputeShader : public Shader
	{
	public:
		ComputeShader(const std::filesystem::path& filePath);
		ID3D11ComputeShader* GetShader() const { return m_Shader.Get(); }
		~ComputeShader();
		void Bind() override;
		static Ref<ComputeShader> Create(const std::filesystem::path& filePath);

	private:
		ComPtr<ID3D11ComputeShader> m_Shader;
		ComPtr<ID3D11UnorderedAccessView> m_UAV;
	};
}