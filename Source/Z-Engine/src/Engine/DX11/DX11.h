#pragma once
#include <wrl.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

namespace Alpine
{
	class DX11
	{
	public:
		static void ClearRenderView();
	private:
		static ComPtr<IDXGISwapChain> m_Swapchain;
		static ComPtr<ID3D11Device> m_Device;
		static ComPtr<ID3D11DeviceContext> m_Context;
		static ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		static ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

	};
}
