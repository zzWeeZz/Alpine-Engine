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
		static void CleanUpDX11();
	private:
		static Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapchain;
		static Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> myDeviceContext;
	};
}