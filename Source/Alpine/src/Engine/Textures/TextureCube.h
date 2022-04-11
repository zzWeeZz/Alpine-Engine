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

		void Bind(unsigned int slot);
		static std::shared_ptr<TextureCube> Create(const std::filesystem::path& filePath);
	private:
		std::array<ID3D11Resource*, 6> myTextureViews;
		ComPtr<ID3D11SamplerState>myTextureSamplerState;
		ComPtr<ID3D11ShaderResourceView> myShaderResourceView;
	};
}
