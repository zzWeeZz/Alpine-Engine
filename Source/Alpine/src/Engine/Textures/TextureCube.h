#pragma once


#include <d3d11.h>
#include <filesystem>
#include <wrl/client.h>
#include <array>


using namespace Microsoft::WRL;

namespace Alpine
{
	class TextureCube
	{
	public:
		TextureCube(const std::filesystem::path& filePath);
		TextureCube(UINT width, UINT height, DXGI_FORMAT format, UINT levels);
		void Bind(unsigned int slot, bool forCompute = false);
		ID3D11Resource* GetTextures();
		ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return m_ShaderResourceView; }
		ComPtr<ID3D11UnorderedAccessView> GetUnorderedAccessView() { return m_UAV; }
		void CreateUAV(UINT levels = 0);
		unsigned int GetLevels();
		static std::shared_ptr<TextureCube> Create(const std::filesystem::path& filePath);
		static std::shared_ptr<TextureCube> Create(UINT width, UINT height, DXGI_FORMAT format, UINT levels = 0);
	private:
		unsigned int m_Levels;
		std::array<ID3D11Resource*, 6> m_TextureViews;
		ID3D11Texture2D* m_Texture;
		ComPtr<ID3D11SamplerState> m_TextureSamplerState;
		ComPtr<ID3D11UnorderedAccessView> m_UAV;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
	};
}
