#pragma once
#include <d3d11.h>
#include <filesystem>
#include "DX11/Utilities.h"
namespace Alpine
{

	class Shader
	{
	public:
		virtual bool Initialize(const std::filesystem::path& filePath) = 0;
		virtual ID3D10Blob* GetBuffer() = 0;
		virtual ~Shader() {}
	protected:
		ComPtr<ID3D10Blob> myBuffer;
	};
}