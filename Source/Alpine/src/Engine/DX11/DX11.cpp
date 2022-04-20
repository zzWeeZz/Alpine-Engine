#include "DX11.h"
#include <cassert>
#include <DirectXColors.h>
#include <iostream>
#include <ostream>

#include "Application/Application.h"

Alpine::DX11 Alpine::DX11::m_Instance;

Alpine::DX11::DX11()
{
	m_Instance = *this;
}

#pragma region Getters
IDXGISwapChain* Alpine::DX11::GetSwapChain()
{
	return m_Instance.m_Swapchain.Get();
}

IDXGISwapChain** Alpine::DX11::GetAdressOfSwapChain()
{
	return m_Instance.m_Swapchain.GetAddressOf();
}

ID3D11Device* Alpine::DX11::GetDevice()
{
	return m_Instance.m_Device.Get();
}

ID3D11Device** Alpine::DX11::GetAdressOfDevice()
{
	return m_Instance.m_Device.GetAddressOf();
}

ID3D11DeviceContext* Alpine::DX11::GetDeviceContext()
{
	return m_Instance.m_DeviceContext.Get();
}

ID3D11DeviceContext** Alpine::DX11::GetAdressOfDeviceContext()
{
	return m_Instance.m_DeviceContext.GetAddressOf();
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Alpine::DX11::GetChainRenderView()
{
	return m_Instance.m_SwapchainRenderTargetView;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Alpine::DX11::GetChainDepthView()
{
	return m_Instance.m_SwapchainDepthStencilView;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> Alpine::DX11::GetChainDepthBuffer()
{
	return m_Instance.m_SwapchainDepthStencilBuffer;
}
#pragma endregion

void Alpine::DX11::Initialize(int32_t width, int32_t height, bool fullscreen)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = Application::GetWindow()->GetHwnd();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		DX11::GetAdressOfSwapChain(),
		DX11::GetAdressOfDevice(),
		NULL,
		DX11::GetAdressOfDeviceContext());

	ID3D11Texture2D* backBuffer;
	auto hr = m_Instance.m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	assert(SUCCEEDED(hr));
	hr = m_Instance.m_Device->CreateRenderTargetView(backBuffer, NULL, &m_Instance.m_SwapchainRenderTargetView);
	assert(SUCCEEDED(hr));
	backBuffer->Release();
	backBuffer = nullptr;

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

	hr = m_Instance.m_Device->CreateTexture2D(&depthStencilDesc, NULL, &m_Instance.m_SwapchainDepthStencilBuffer);
	assert(SUCCEEDED(hr));
	hr = m_Instance.m_Device->CreateDepthStencilView(m_Instance.m_SwapchainDepthStencilBuffer.Get(), NULL, &m_Instance.m_SwapchainDepthStencilView);
	assert(SUCCEEDED(hr));

	m_Instance.m_DeviceContext->OMSetRenderTargets(1, m_Instance.m_SwapchainRenderTargetView.GetAddressOf(), m_Instance.m_SwapchainDepthStencilView.Get());

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Height = Application::GetWindow()->GetHeight();
	viewport.Width = Application::GetWindow()->GetWidth();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	DX11::GetDeviceContext()->RSSetViewports(1, &viewport);
}

void Alpine::DX11::Resize(int width, int height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	assert(m_Instance.m_DeviceContext);
	assert(m_Instance.m_Device);
	assert(m_Instance.m_Swapchain);

	m_Instance.m_SwapchainDepthStencilBuffer.Reset();
	m_Instance.m_SwapchainDepthStencilView.Reset();
	m_Instance.m_SwapchainRenderTargetView.Reset();

	ID3D11RenderTargetView* nullViews[] = { nullptr, nullptr };
	m_Instance.m_DeviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_Instance.m_DeviceContext->Flush();
	//Resize swap chain
	auto hr = m_Instance.m_Swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);
	assert(SUCCEEDED(hr));
	
	////Create new views
	ID3D11Texture2D* backBuffer;
	hr = m_Instance.m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer));
	assert(SUCCEEDED(hr));
	hr = m_Instance.m_Device->CreateRenderTargetView(backBuffer, 0, &m_Instance.m_SwapchainRenderTargetView);
	assert(SUCCEEDED(hr));
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


	hr = m_Instance.m_Device->CreateTexture2D(&depthStencilDesc, 0, m_Instance.m_SwapchainDepthStencilBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = m_Instance.m_Device->CreateDepthStencilView(m_Instance.m_SwapchainDepthStencilBuffer.Get(), 0, m_Instance.m_SwapchainDepthStencilView.GetAddressOf());
	assert(SUCCEEDED(hr));
	m_Instance.m_DeviceContext->OMSetRenderTargets(1, m_Instance.m_SwapchainRenderTargetView.GetAddressOf(), m_Instance.m_SwapchainDepthStencilView.Get());
	//Bind to output merger

	//Set viewport
	D3D11_VIEWPORT viewportDesc = {};
	viewportDesc.TopLeftX = 0.f;
	viewportDesc.TopLeftY = 0.f;
	viewportDesc.Width = static_cast<float>(width);
	viewportDesc.Height = static_cast<float>(height);
	viewportDesc.MinDepth = 0.f;
	viewportDesc.MaxDepth = 1.f;

	m_Instance.m_DeviceContext->RSSetViewports(1, &viewportDesc);
}

void Alpine::DX11::ClearView()
{
	m_Instance.m_DeviceContext->ClearRenderTargetView(m_Instance.m_SwapchainRenderTargetView.Get(), DirectX::Colors::Cyan);
	m_Instance.m_DeviceContext->ClearDepthStencilView(m_Instance.m_SwapchainDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Alpine::DX11::Bind()
{
	m_Instance.m_DeviceContext->OMSetRenderTargets(1, m_Instance.m_SwapchainRenderTargetView.GetAddressOf(), m_Instance.m_SwapchainDepthStencilView.Get());
}

void Alpine::DX11::Present(const bool& vsync)
{
	if(vsync)
	{
		m_Instance.m_Swapchain->Present(1, 0);
	}
	else
	{
		m_Instance.m_Swapchain->Present(0, 0);
	}
}

void Alpine::DX11::CleanUpDX11()
{
	m_Instance.m_Swapchain.Reset();
	m_Instance.m_Device.Reset();
	m_Instance.m_DeviceContext.Reset();
}
