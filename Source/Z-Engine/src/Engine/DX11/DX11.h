#pragma once
#include <d3d11.h>
#include <wrl/client.h>


class DX11
{
public:
	static Microsoft::WRL::ComPtr<IDXGISwapChain>& GetSwapChain();
	static Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice();
private:
	static Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapchain;
	static Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
};
