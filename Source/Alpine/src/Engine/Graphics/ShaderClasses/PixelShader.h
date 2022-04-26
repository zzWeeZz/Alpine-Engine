#pragma once
#include "DX11/DX11.h"
#include "DX11/Utilities.h"
#include "Shader.h"
namespace Alpine
{
	class PixelShader
	{
	public:
		PixelShader(const std::filesystem::path& filePath);

		void Bind();
		void Unbind();

		static Ref<PixelShader> Create(const std::filesystem::path& filePath);

	private:
		ComPtr<ID3D11PixelShader> m_Shader;
		ComPtr<ID3D10Blob> m_Buffer;
	};
}
