#pragma once
#include "Alpine/DX11/Utilities.h"
#include <d3d11.h>
namespace Alpine
{
	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		std::vector<DXGI_FORMAT> colorFormat = {DXGI_FORMAT_R8G8B8A8_UNORM};
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FramebufferSpecification& spec);

		void ClearView(const Color& clearColor);
		void ClearDepthStencil();
		void Bind();
		void UnBind();
		void Resize(uint32_t width, uint32_t height);
		ComPtr<ID3D11ShaderResourceView> GetColorAttachment() { return m_ShaderResourceViews[0].Get(); };
		FramebufferSpecification& GetSpecification() { return m_Specification; };

		~FrameBuffer();
		
		static Ref<FrameBuffer> Create(const FramebufferSpecification& specs);
	private:
		void Invalidate();

		std::vector<ComPtr<ID3D11Texture2D>> m_ColorBuffers;
		std::vector<ComPtr<ID3D11RenderTargetView>> m_RenderTargetViews;
		std::vector<ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;
		ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		D3D11_VIEWPORT m_Viewport = {};

		FramebufferSpecification m_Specification;
	};
}