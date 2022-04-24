#pragma once
#include <cstdint>
#include <d3d11.h>
#include "DX11/Utilities.h"
#include "RenderStateManager.h"
namespace Alpine
{
	class DX11
	{
	public:
		static ComPtr<IDXGISwapChain>& SwapChain();
		static ComPtr<ID3D11Device>& Device();
		static ComPtr<ID3D11DeviceContext>& Context();
		static ComPtr<ID3D11RenderTargetView>& SwapChainRenderView();
		static ComPtr<ID3D11DepthStencilView>& SwapChainDepthView();
		static ComPtr<ID3D11Texture2D>& SwapChainDepthBuffer();
		static RenderStateManager& GetRenderStateManager();

		DX11();
		static void Initialize(int32_t width, int32_t height, bool fullscreen);
		static void Resize(int width, int height);
		static void ClearView();
		static void Bind();
		static void Present(const bool& vsync);

		static void CleanUpDX11();
	private:
		static DX11 s_Instance;


		RenderStateManager m_RenderStateManager;
		ComPtr<IDXGISwapChain> m_Swapchain;
		ComPtr<ID3D11Device> m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		ComPtr<ID3D11RenderTargetView> m_SwapchainRenderTargetView;
		ComPtr<ID3D11DepthStencilView> m_SwapchainDepthStencilView;
		ComPtr<ID3D11Texture2D> m_SwapchainDepthStencilBuffer;
	};
}