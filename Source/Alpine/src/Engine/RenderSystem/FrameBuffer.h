#pragma once
#include "DirectXTK/SimpleMath.h"
#include <d3d11.h>

using namespace DirectX::SimpleMath;
namespace Alpine
{
	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		
		bool swapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FramebufferSpecification& spec);

		void ClearView(const Color& clearColor);
		void ClearDepthStencil();
		void Bind();
		void UnBind();
		
		ID3D11ShaderResourceView* GetColorAttachment() { return m_ShaderResourceView; };
		FramebufferSpecification& GetSpecification() { return m_Specification; };

		~FrameBuffer();
		
		static std::shared_ptr<FrameBuffer> Create(const FramebufferSpecification& specs);
	private:
		void Invalidate();

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11ShaderResourceView* m_ShaderResourceView;
		
		FramebufferSpecification m_Specification;
	};
}