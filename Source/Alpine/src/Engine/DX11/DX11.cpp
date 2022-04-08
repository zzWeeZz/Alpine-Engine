#include "DX11.h"
#include <cassert>

Alpine::DX11 Alpine::DX11::myInstance;

Alpine::DX11::DX11()
{
	myInstance = *this;
}

IDXGISwapChain* Alpine::DX11::GetSwapChain()
{
	return myInstance.mySwapchain.Get();
}

IDXGISwapChain** Alpine::DX11::GetAdressOfSwapChain()
{
	return myInstance.mySwapchain.GetAddressOf();
}

ID3D11Device* Alpine::DX11::GetDevice()
{
	return myInstance.myDevice.Get();
}

ID3D11Device** Alpine::DX11::GetAdressOfDevice()
{
	return myInstance.myDevice.GetAddressOf();
}

ID3D11DeviceContext* Alpine::DX11::GetDeviceContext()
{
	return myInstance.myDeviceContext.Get();
}

ID3D11DeviceContext** Alpine::DX11::GetAdressOfDeviceContext()
{
	return myInstance.myDeviceContext.GetAddressOf();
}

ID3D11RenderTargetView* Alpine::DX11::GetRenderTargetView()
{
	return myInstance.myRenderTargetView.Get();
}

ID3D11RenderTargetView** Alpine::DX11::GetAdressOfRenderTargetView()
{
	return myInstance.myRenderTargetView.GetAddressOf();
}

ID3D11DepthStencilView* Alpine::DX11::GetDepthStencilView()
{
	return myInstance.myDepthStencilView.Get();
}

ID3D11DepthStencilView** Alpine::DX11::GetAdressOfDepthStencilView()
{
	return myInstance.myDepthStencilView.GetAddressOf();
}

ID3D11Texture2D* Alpine::DX11::GetDepthStencilBuffer()
{
	return myInstance.myDepthStencilBuffer.Get();
}

ID3D11Texture2D** Alpine::DX11::GetAdressOfDepthStencilBuffer()
{
	return myInstance.myDepthStencilBuffer.GetAddressOf();
}

void Alpine::DX11::Resize(int width, int height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	assert(myInstance.myDeviceContext);
	assert(myInstance.myDevice);
	assert(myInstance.mySwapchain);

	myInstance.myRenderTargetView.Reset();
	myInstance.myDepthStencilView.Reset();
	myInstance.myDepthStencilBuffer.Reset();
	//Resize swap chain
	myInstance.mySwapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	//Create new views
	ID3D11Texture2D* backBuffer;
	myInstance.mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer));
	myInstance.myDevice->CreateRenderTargetView(backBuffer, 0, myInstance.myRenderTargetView.GetAddressOf());
	backBuffer->Release();
	//Create depth stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.MiscFlags = 0;


	myInstance.myDevice->CreateTexture2D(&depthStencilDesc, 0, myInstance.myDepthStencilBuffer.GetAddressOf());
	myInstance.myDevice->CreateDepthStencilView(myInstance.myDepthStencilBuffer.Get(), 0, myInstance.myDepthStencilView.GetAddressOf());

	myInstance.myDeviceContext->OMSetRenderTargets(1, myInstance.myRenderTargetView.GetAddressOf(), myInstance.myDepthStencilView.Get());
	//Bind to output merger

	//Set viewport
	D3D11_VIEWPORT viewportDesc = {};
	viewportDesc.TopLeftX = 0.f;
	viewportDesc.TopLeftY = 0.f;
	viewportDesc.Width = static_cast<float>(width);
	viewportDesc.Height = static_cast<float>(height);
	viewportDesc.MinDepth = 0.f;
	viewportDesc.MaxDepth = 1.f;

	myInstance.myDeviceContext->RSSetViewports(1, &viewportDesc);
}


void Alpine::DX11::CleanUpDX11()
{
	myInstance.myRenderTargetView.Reset();
	myInstance.myDepthStencilView.Reset();
	myInstance.mySwapchain.Reset();
	myInstance.myDevice.Reset();
	myInstance.myDeviceContext.Reset();
}
