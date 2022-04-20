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


		static void Initialize(int32_t width, int32_t height, bool fullscreen);

		static void Resize(int width, int height);

		static void Present(const bool& vsync);

		static void CleanUpDX11();
	private:
		static DX11 m_Instance;

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_Swapchain;
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	};
}