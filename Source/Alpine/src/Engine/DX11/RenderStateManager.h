#pragma once
#include <d3d11.h>
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
		NoBlend,
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
		void SetRasterizerState(CullMode cullMode);
		void SetBlendState(BlendMode blendMode);
		void SetDepthStencilState(DepthStencilMode depthStencilMode);
		void SetSamplerState(SamplerMode samplerMode, ShaderType shaderType, uint8_t slot = 0);
	private:
		std::unordered_map<CullMode, ComPtr<ID3D11RasterizerState>> m_RasterizerStates;
		std::unordered_map<BlendMode, ComPtr<ID3D11BlendState>> m_BlendStates;
		std::unordered_map<SamplerMode,ComPtr<ID3D11SamplerState>> m_SamplerStates;
		std::unordered_map<DepthStencilMode, ComPtr<ID3D11DepthStencilState>> m_DepthStencilStates;
	};
}