#pragma once
#include "Shader.h"
#include "Engine/DX11/DX11.h"
#include "Engine/DX11/Utilities.h"
namespace Alpine
{
	class PixelShader : public Shader
	{
	public:
		PixelShader(const std::filesystem::path& filePath);

		void Bind() override;
		void Unbind();

		static Ref<PixelShader> Create(const std::filesystem::path& filePath);

	private:
		ComPtr<ID3D11PixelShader> m_Shader;
		ComPtr<ID3D10Blob> m_Buffer;
	};
}
