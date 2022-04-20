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
}

void Alpine::FrameBuffer::UnBind()
{
	DX11::GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);
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
	auto hr = DX11::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&ptrSurface);
	assert(SUCCEEDED(hr));
	hr = DX11::GetDevice()->CreateRenderTargetView(ptrSurface, nullptr, &m_RenderTargetView);
	assert(SUCCEEDED(hr));
	ptrSurface->Release();
	ID3D11Texture2D* ptrshaderResourceView;
	m_RenderTargetView->GetResource((ID3D11Resource**)&ptrshaderResourceView);
	
	hr = DX11::GetDevice()->CreateShaderResourceView(ptrSurface, nullptr, &m_ShaderResourceView);
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
}