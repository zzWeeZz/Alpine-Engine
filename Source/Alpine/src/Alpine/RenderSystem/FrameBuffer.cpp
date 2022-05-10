#include "FrameBuffer.h"
#include "Alpine/DX11/DX11.h"
#include "Application/Application.h"


Alpine::FrameBuffer::FrameBuffer(const FramebufferSpecification& spec) : m_Specification(spec)
{
	m_DepthStencilBuffer = nullptr;
	m_DepthStencilView = nullptr;
	m_RenderTargetView = nullptr;
	m_ShaderResourceView = nullptr;
	Invalidate();
}

void Alpine::FrameBuffer::ClearView(const Color& clearColor)
{
	float color[4] = { clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A()};
	DX11::Context()->ClearRenderTargetView(m_RenderTargetView, color);
}

void Alpine::FrameBuffer::ClearDepthStencil()
{
	DX11::Context()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Alpine::FrameBuffer::Bind()
{
	DX11::Context()->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	DX11::Context()->RSSetViewports(1, &m_Viewport);
}

void Alpine::FrameBuffer::UnBind()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	DX11::Context()->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	DX11::Context()->Flush();
	DX11::Bind();
}

void Alpine::FrameBuffer::Resize(uint32_t width, uint32_t height)
{
	m_Specification.width = width;
	m_Specification.height = height;
	Invalidate();
}

Alpine::FrameBuffer::~FrameBuffer()
{
	m_ShaderResourceView->Release();
	m_DepthStencilBuffer->Release();
	m_DepthStencilView->Release();
	m_RenderTargetView->Release();
}

Alpine::Ref<Alpine::FrameBuffer> Alpine::FrameBuffer::Create(const FramebufferSpecification& specs)
{
	return std::make_shared<FrameBuffer>(specs);
}

void Alpine::FrameBuffer::Invalidate()
{
	if(m_ShaderResourceView) m_ShaderResourceView->Release();
	if (m_DepthStencilBuffer) m_DepthStencilBuffer->Release();
	if (m_DepthStencilView) m_DepthStencilView->Release();
	if (m_RenderTargetView) m_RenderTargetView->Release();

	ID3D11Texture2D* ptrSurface;
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = m_Specification.width;
	desc.Height = m_Specification.height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	auto hr = DX11::Device()->CreateTexture2D(&desc, nullptr, &ptrSurface);
	assert(SUCCEEDED(hr));
	// map ptrSurface
	m_ColorBuffer = ptrSurface;
	hr = DX11::Device()->CreateRenderTargetView(m_ColorBuffer, nullptr, &m_RenderTargetView);
	assert(SUCCEEDED(hr));
	hr = DX11::Device()->CreateShaderResourceView(m_ColorBuffer, nullptr, &m_ShaderResourceView);
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
	hr = DX11::Device()->CreateTexture2D(&depthStencilDesc, nullptr, &m_DepthStencilBuffer);
	assert(SUCCEEDED(hr));
	hr = DX11::Device()->CreateDepthStencilView(m_DepthStencilBuffer, nullptr, &m_DepthStencilView);
	assert(SUCCEEDED(hr));

	m_Viewport.Width = (float)m_Specification.width;
	m_Viewport.Height = (float)m_Specification.height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
}