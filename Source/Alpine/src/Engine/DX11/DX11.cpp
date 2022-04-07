#include "DX11.h"
#include <cassert>

Microsoft::WRL::ComPtr<IDXGISwapChain> Alpine::DX11::mySwapchain;
Microsoft::WRL::ComPtr<ID3D11Device> Alpine::DX11::myDevice;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Alpine::DX11::myDeviceContext;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Alpine::DX11::myRenderTargetView;
Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Alpine::DX11::myDepthStencilView;
Microsoft::WRL::ComPtr<ID3D11Texture2D> Alpine::DX11::myDepthStencilBuffer;
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

ID3D11RenderTargetView* Alpine::DX11::GetRenderTargetView()
{
	return myRenderTargetView.Get();
}

ID3D11RenderTargetView** Alpine::DX11::GetAdressOfRenderTargetView()
{
	return myRenderTargetView.GetAddressOf();
}

ID3D11DepthStencilView* Alpine::DX11::GetDepthStencilView()
{
	return myDepthStencilView.Get();
}

ID3D11DepthStencilView** Alpine::DX11::GetAdressOfDepthStencilView()
{
	return myDepthStencilView.GetAddressOf();
}

ID3D11Texture2D* Alpine::DX11::GetDepthStencilBuffer()
{
	return myDepthStencilBuffer.Get();
}

ID3D11Texture2D** Alpine::DX11::GetAdressOfDepthStencilBuffer()
{
	return myDepthStencilBuffer.GetAddressOf();
}

void Alpine::DX11::Resize(int width, int height)
{
	if (width == 0 || height == 0)
	{
		
		return;
	}

	assert(myDeviceContext);
	assert(myDevice);
	assert(mySwapchain);


	myRenderTargetView.ReleaseAndGetAddressOf();
	myDepthStencilView.ReleaseAndGetAddressOf();
	myDepthStencilBuffer.ReleaseAndGetAddressOf();
	//Resize swap chain
	mySwapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	//Create new views
	ID3D11Texture2D* backBuffer;
	mySwapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	myDevice->CreateRenderTargetView(backBuffer, 0, myRenderTargetView.GetAddressOf());
	backBuffer->Release();
	//Create depth stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.MiscFlags = 0;

	myDevice->CreateTexture2D(&depthStencilDesc, 0, myDepthStencilBuffer.GetAddressOf());
	myDevice->CreateDepthStencilView(myDepthStencilBuffer.Get(), 0, myDepthStencilView.GetAddressOf());

	myDeviceContext->OMSetRenderTargets(1, myRenderTargetView.GetAddressOf(), myDepthStencilView.Get());
	//Bind to output merger

	//Set viewport
	D3D11_VIEWPORT viewportDesc = {};
	viewportDesc.TopLeftX = 0.f;
	viewportDesc.TopLeftY = 0.f;
	viewportDesc.Width = static_cast<float>(width);
	viewportDesc.Height = static_cast<float>(height);
	viewportDesc.MinDepth = 0.f;
	viewportDesc.MaxDepth = 1.f;

	myDeviceContext->RSSetViewports(1, &viewportDesc);
}


void Alpine::DX11::CleanUpDX11()
{
	myRenderTargetView.Reset();
	myDepthStencilView.Reset();
	mySwapchain.Reset();
	myDevice.Reset();
	myDeviceContext.Reset();
}
