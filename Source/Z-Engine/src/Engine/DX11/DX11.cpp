#include "DX11.h"


Microsoft::WRL::ComPtr<IDXGISwapChain>& DX11::GetSwapChain()
{
	return DX11::mySwapchain;
}