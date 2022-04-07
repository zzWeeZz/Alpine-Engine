#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace Alpine
{
	class DX11
	{
	public:
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

		static void Resize(int width, int height);

		static void CleanUpDX11();
	private:
		static Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapchain;
		static Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> myDeviceContext;
		static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> myRenderTargetView;
		static Microsoft::WRL::ComPtr<ID3D11DepthStencilView> myDepthStencilView;
		static Microsoft::WRL::ComPtr<ID3D11Texture2D> myDepthStencilBuffer;
	};
}