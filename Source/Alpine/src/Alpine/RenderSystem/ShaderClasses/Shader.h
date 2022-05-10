#pragma once
#include <d3d11.h>
#include <filesystem>
#include "Alpine/DX11/Utilities.h"
namespace Alpine
{

	class Shader
	{
	public:
		virtual void Bind() = 0;
		virtual ComPtr<ID3D10Blob> GetBuffer() { return m_Buffer; }
		virtual ~Shader() {}
	protected:
		ComPtr<ID3D10Blob> m_Buffer;
	};
}