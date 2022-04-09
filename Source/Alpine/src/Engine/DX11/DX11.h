#pragma once
#include <cstdint>
#include <d3d11.h>
#include <wrl/client.h>

namespace Alpine
{
	class DX11
	{
	public:
		DX11();


		static IDXGISwapChain* GetSwapChain();
		static IDXGISwapChain** GetAdressOfSwapChain();

		static ID3D11Device* GetDevice();
		static ID3D11Device** GetAdressOfDevice();

		static ID3D11DeviceContext* GetDeviceContext();
		static ID3D11DeviceContext** GetAdressOfDeviceContext();

		static ID3D11RenderTargetView* GetRenderTargetView();
		static ID3D11RenderTargetView** GetAdressOfRenderTargetView();

		static ID3D11DepthStencilView* GetDepthStencilView();
		static ID3D11DepthStencilView** GetAdressOfDepthStencilView();

		static ID3D11Texture2D* GetDepthStencilBuffer();
		static ID3D11Texture2D** GetAdressOfDepthStencilBuffer();

		static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetRenderTexture();

		static void Initialize(int32_t width, int32_t height, bool fullscreen);

		static void Resize(int width, int height);

		static void ClearView();

		static void Present(const bool& vsync);

		static void CleanUpDX11();
	private:
		static DX11 myInstance;

		Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapchain;
		Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> myDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> myRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> myDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> myDepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myRenderTexture;
	};
}