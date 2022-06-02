#include "FrameBuffer.h"
#include "Alpine/DX11/DX11.h"
#include "Alpine/DX11/Utilities.h"
#include "Application/Application.h"


Alpine::FrameBuffer::FrameBuffer(const FramebufferSpecification& spec) : m_Specification(spec)
{
	m_DepthStencilBuffer = nullptr;
	m_DepthStencilView = nullptr;
	m_RenderTargetViews.resize(spec.colorFormat.size());
	m_ShaderResourceViews.resize(spec.colorFormat.size());
	Invalidate();
}

void Alpine::FrameBuffer::ClearView(const Color& clearColor)
{
	float color[4] = { clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A() };
	DX11::Context()->ClearRenderTargetView(m_RenderTargetViews.data()->Get(), color);
}

void Alpine::FrameBuffer::ClearDepthStencil()
{
	DX11::Context()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Alpine::FrameBuffer::Bind()
{
	DX11::Context()->OMSetRenderTargets(m_RenderTargetViews.size(), m_RenderTargetViews.data()->GetAddressOf(), m_DepthStencilView.Get());
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
	/*m_ShaderResourceViews->Release();
	m_DepthStencilBuffer->Release();
	m_DepthStencilView->Release();
	m_RenderTargetView->Release();*/
}

Alpine::Ref<Alpine::FrameBuffer> Alpine::FrameBuffer::Create(const FramebufferSpecification& specs)
{
	return std::make_shared<FrameBuffer>(specs);
}

void Alpine::FrameBuffer::Invalidate()
{
	if (m_DepthStencilBuffer) m_DepthStencilBuffer->Release();
	if (m_DepthStencilView) m_DepthStencilView->Release();
	for (auto& rtv : m_RenderTargetViews)
	{
		if (rtv) rtv->Release();
	}
	for (auto& srv : m_ShaderResourceViews)
	{
		if (srv)srv->Release();
	}

	for (size_t i = 0; i < m_Specification.colorFormat.size(); i++)
	{
		if (m_Specification.colorFormat[i] != DXGI_FORMAT_D24_UNORM_S8_UINT)
		{
			ID3D11Texture2D* ptrSurface;
			D3D11_TEXTURE2D_DESC desc = {};
			desc.Width = m_Specification.width;
			desc.Height = m_Specification.height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = m_Specification.colorFormat[i];
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			auto hr = DX11::Device()->CreateTexture2D(&desc, nullptr, &ptrSurface);
			assert(SUCCEEDED(hr));
			// map ptrSurface
			m_ColorBuffers.emplace_back(ptrSurface);
			AssertIfFailed(DX11::Device()->CreateRenderTargetView(m_ColorBuffers[i].Get(), nullptr, m_RenderTargetViews[i].GetAddressOf()));
			AssertIfFailed(DX11::Device()->CreateShaderResourceView(m_ColorBuffers[i].Get(), nullptr, m_ShaderResourceViews[i].GetAddressOf()));
		}
		else
		{
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
			AssertIfFailed(DX11::Device()->CreateTexture2D(&depthStencilDesc, nullptr, m_DepthStencilBuffer.GetAddressOf()));
			AssertIfFailed(DX11::Device()->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf()));
		}
	}

	m_Viewport.Width = (float)m_Specification.width;
	m_Viewport.Height = (float)m_Specification.height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
}