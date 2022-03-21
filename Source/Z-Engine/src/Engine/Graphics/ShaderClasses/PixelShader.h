#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>

class PixelShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D10InputLayout* GetInputLayout();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> myShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> myBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> myInputLayout;
};