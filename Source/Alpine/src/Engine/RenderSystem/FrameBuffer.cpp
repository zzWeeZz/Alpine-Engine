#include "FrameBuffer.h"
#include "DX11/DX11.h"
#include "Application/Application.h"


Alpine::FrameBuffer::FrameBuffer(const FramebufferSpecification& spec) : m_Specification(spec)
{
	Invalidate();
}

void Alpine::FrameBuffer::ClearView(const Color& clearColor)
{
	float color[4] = { clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A()};
	DX11::GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView, color);
}

void Alpine::FrameBuffer::ClearDepthStencil()
{
	DX11::GetDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Alpine::FrameBuffer::Bind()
{
	DX11::GetDeviceContext()->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	DX11::GetDeviceContext()->RSSetViewports(1, &m_Viewport);
}

void Alpine::FrameBuffer::UnBind()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	DX11::GetDeviceContext()->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	DX11::GetDeviceContext()->Flush();
}

Alpine::FrameBuffer::~FrameBuffer()
{
	m_ShaderResourceView->Release();
	m_DepthStencilBuffer->Release();
	m_DepthStencilView->Release();
	m_RenderTargetView->Release();
}

std::shared_ptr<Alpine::FrameBuffer> Alpine::FrameBuffer::Create(const FramebufferSpecification& specs)
{
	return std::make_shared<FrameBuffer>(specs);
}

void Alpine::FrameBuffer::Invalidate()
{
	ID3D11Texture2D* ptrSurface;
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = m_Specification.width;
	desc.Height = m_Specification.height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	auto hr = DX11::GetDevice()->CreateTexture2D(&desc, nullptr, &ptrSurface);
	assert(SUCCEEDED(hr));
	// map ptrSurface
	m_ColorBuffer = ptrSurface;
	hr = DX11::GetDevice()->CreateRenderTargetView(m_ColorBuffer, nullptr, &m_RenderTargetView);
	assert(SUCCEEDED(hr));
	hr = DX11::GetDevice()->CreateShaderResourceView(m_ColorBuffer, nullptr, &m_ShaderResourceView);
	assert(SUCCEEDED(hr));
	
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = m_Specification.width;
	depthStencilDesc.Height = m_Specification.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = m_Specification.samples;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.MiscFlags = 0;
	hr = DX11::GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &m_DepthStencilBuffer);
	assert(SUCCEEDED(hr));
	hr = DX11::GetDevice()->CreateDepthStencilView(m_DepthStencilBuffer, nullptr, &m_DepthStencilView);
	assert(SUCCEEDED(hr));

	m_Viewport.Width = (float)m_Specification.width;
	m_Viewport.Height = (float)m_Specification.height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
}