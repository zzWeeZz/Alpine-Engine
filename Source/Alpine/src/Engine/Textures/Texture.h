#pragma once
#include <d3d11.h>
#include <filesystem>
#include "Engine/DX11/Utilities.h"
namespace Alpine
{
	class Texture
	{
	public:
		Texture(const std::filesystem::path& aPath);
		Texture(UINT width, UINT height, DXGI_FORMAT format, UINT levels);
		void Bind(unsigned int slot, bool useCompute = false);

		void CreateUAV(UINT levels = 0);

		ComPtr<ID3D11Resource> GetResource();
		ComPtr<ID3D11UnorderedAccessView> GetUnorderedAccessView();
		ComPtr<ID3D11ShaderResourceView> GetShaderResourceView();
		static Ref<Texture> Create(const std::filesystem::path& aPath);
		static Ref<Texture> Create(UINT width, UINT height, DXGI_FORMAT format, UINT levels = 0);
	private:
		void CreateTextureFromHdrData();
		ComPtr<ID3D11Texture2D> m_Texture;
		ComPtr< ID3D11Resource> m_Resource;
		ComPtr<ID3D11UnorderedAccessView> m_UAV;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
		bool m_IsHDR = false;
		int m_Height = 0;
		int m_Width = 0;
		int m_Channels = 0;
		std::unique_ptr<unsigned char> m_Pixels;
	};
}