#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>

class PixelShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath);
	ID3D11PixelShader* GetShader() const;
	ID3D10Blob* GetBuffer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> myShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> myBuffer;
};