#include "DX11.h"

Microsoft::WRL::ComPtr<IDXGISwapChain> Alpine::DX11::mySwapchain;
Microsoft::WRL::ComPtr<ID3D11Device> Alpine::DX11::myDevice;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Alpine::DX11::myDeviceContext;
IDXGISwapChain* Alpine::DX11::GetSwapChain()
{
	return mySwapchain.Get();
}

IDXGISwapChain** Alpine::DX11::GetAdressOfSwapChain()
{
	return mySwapchain.GetAddressOf();
}

ID3D11Device* Alpine::DX11::GetDevice()
{
	return myDevice.Get();
}

ID3D11Device** Alpine::DX11::GetAdressOfDevice()
{
	return myDevice.GetAddressOf();
}

ID3D11DeviceContext* Alpine::DX11::GetDeviceContext()
{
	return myDeviceContext.Get();
}

ID3D11DeviceContext** Alpine::DX11::GetAdressOfDeviceContext()
{
	return myDeviceContext.GetAddressOf();
}

void Alpine::DX11::CleanUpDX11()
{
	mySwapchain.Get()->Release();
	myDevice.Get()->Release();
	myDeviceContext.Get()->Release();
}
