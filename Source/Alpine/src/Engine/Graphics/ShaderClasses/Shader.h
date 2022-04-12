#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include "DX11/DX11.h"
#include <filesystem>
namespace Alpine
{

	class Shader
	{
	public:
		virtual bool Initialize(const std::filesystem::path& filePath) = 0;
		virtual ID3D10Blob* GetBuffer() = 0;
		virtual ~Shader() {}
	protected:
		Microsoft::WRL::ComPtr<ID3D10Blob> myBuffer;
	};
}