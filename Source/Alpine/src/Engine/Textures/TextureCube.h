#pragma once


#include <d3d11.h>
#include <filesystem>
#include "DX11/Utilities.h"
#include <array>

namespace Alpine
{
	class TextureCube
	{
	public:
		TextureCube(const std::filesystem::path& filePath);
		TextureCube(UINT width, UINT height, DXGI_FORMAT format, UINT levels);
		void Bind(unsigned int slot, bool forCompute = false);
		void GenerateMipMaps();
		void CreateUAV(UINT levels = 0);
		unsigned int GetLevels();

		ComPtr<ID3D11Texture2D> GetTextures();
		ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return m_ShaderResourceView; }
		ComPtr<ID3D11UnorderedAccessView> GetUnorderedAccessView() { return m_UAV; }
		static Ref<TextureCube> Create(const std::filesystem::path& filePath);
		static Ref<TextureCube> Create(UINT width, UINT height, DXGI_FORMAT format, UINT levels = 0);
	private:
		unsigned int m_Levels;
		std::array<ID3D11Resource*, 6> m_TextureViews;
		ComPtr<ID3D11Texture2D> m_Texture;
		ComPtr<ID3D11SamplerState> m_TextureSamplerState;
		ComPtr<ID3D11UnorderedAccessView> m_UAV;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
	};
}
