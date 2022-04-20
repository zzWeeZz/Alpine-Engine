#pragma once
#include <d3d11.h>
#include <filesystem>
#include <wrl/client.h>
using namespace Microsoft::WRL;
namespace Alpine
{
	class Texture
	{
	public:
		Texture(const std::filesystem::path& aPath);
		void Bind(unsigned int slot);
		ComPtr< ID3D11Resource> GetResource();
		ComPtr<ID3D11ShaderResourceView> GetShaderResourceView();
		static std::shared_ptr<Texture> Create(const std::filesystem::path& aPath);
	private:
		ComPtr<ID3D11Texture2D> myTexture;
		ComPtr< ID3D11Resource> myResource;
		ComPtr<ID3D11ShaderResourceView> myShaderResourceView;
	};
}