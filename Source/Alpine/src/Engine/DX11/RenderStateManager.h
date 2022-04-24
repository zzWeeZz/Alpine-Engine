#pragma once
#include <d3d11.h>
#include <stack>

#include "DX11/Utilities.h"

namespace Alpine
{
	enum class ShaderType
	{
		Vertex,
		Pixel,
		Geometry,
		Hull,
		Domain,
		Compute
	};

	enum class CullMode
	{
		None,
		Front,
		Back,
		Wireframe,
	};
	enum class BlendMode
	{
		Opaque,
		AlphaBlend,
		Additive,
		AlphaAdditive,
		Subtractive,
		AlphaSubtractive,
		Multiplicative,
		AlphaMultiplicative,
		NonPremultiplied,
		AlphaNonPremultiplied,
	};
	enum class SamplerMode
	{
		Wrap,
		Mirror,
		Border,
		Point,
		Clamp,
	};

	enum class DepthStencilMode
	{
		None,
		ReadOnly,
		ReadWrite,
	};

	class RenderStateManager
	{
	public:
		void Initialize();
		void PushRasterizerState(CullMode cullMode);
		void PopRasterizerState();

		void PushBlendState(BlendMode blendMode);
		void PopBlendState();

		void PushDepthStencilState(DepthStencilMode depthStencilMode);
		void PopDepthStencilState();

		void SetSamplerState(SamplerMode samplerMode, ShaderType shaderType, uint8_t slot = 0);
	private:

		void UpdateRasterizerState();
		void UpdateBlendState();
		void UpdateDepthStencilState();

		std::unordered_map<CullMode, ComPtr<ID3D11RasterizerState>> m_RasterizerStates;
		std::stack<ComPtr<ID3D11RasterizerState>> m_RasterizerStateStack;

		std::unordered_map<BlendMode, ComPtr<ID3D11BlendState>> m_BlendStates;
		std::stack<ComPtr<ID3D11BlendState>> m_BlendStateStack;

		std::unordered_map<SamplerMode,ComPtr<ID3D11SamplerState>> m_SamplerStates;

		std::unordered_map<DepthStencilMode, ComPtr<ID3D11DepthStencilState>> m_DepthStencilStates;
		std::stack<ComPtr<ID3D11DepthStencilState>> m_DepthStencilStateStack;
	};
}