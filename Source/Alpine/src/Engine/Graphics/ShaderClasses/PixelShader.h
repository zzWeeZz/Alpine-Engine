#pragma once
#include "DX11/DX11.h"
#include "Shader.h"
namespace Alpine
{
	class PixelShader : Shader
	{
	public:
		bool Initialize(const std::filesystem::path& filePath) override;
		ID3D11PixelShader* GetShader() const;
		ID3D10Blob* GetBuffer() override;
		void Bind();

	private:
		ComPtr<ID3D11PixelShader> myShader;
		ComPtr<ID3D10Blob> myBuffer;
	};
}
