#include "RenderStateManager.h"
#include "Alpine/DX11/DX11.h"

void Alpine::RenderStateManager::Initialize()
{
	// Rasterizer states
	D3D11_RASTERIZER_DESC cmDesc = {};
	cmDesc.FillMode = D3D11_FILL_SOLID;
	cmDesc.CullMode = D3D11_CULL_BACK;
	cmDesc.FrontCounterClockwise = true;
	DX11::Device()->CreateRasterizerState(&cmDesc, &m_RasterizerStates[CullMode::Back]);
	cmDesc.CullMode = D3D11_CULL_FRONT;
	DX11::Device()->CreateRasterizerState(&cmDesc, &m_RasterizerStates[CullMode::Front]);
	cmDesc.CullMode = D3D11_CULL_NONE;
	DX11::Device()->CreateRasterizerState(&cmDesc, &m_RasterizerStates[CullMode::None]);
	cmDesc.FillMode = D3D11_FILL_WIREFRAME;
	DX11::Device()->CreateRasterizerState(&cmDesc, &m_RasterizerStates[CullMode::Wireframe]);

	// blend states
	D3D11_BLEND_DESC blendDesc = {};
	D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	renderTargetBlendDesc.BlendEnable = true;
	renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = renderTargetBlendDesc;
	DX11::Device()->CreateBlendState(&blendDesc, &m_BlendStates[BlendMode::AlphaBlend]);

	renderTargetBlendDesc.BlendEnable = false;
	blendDesc.RenderTarget[0] = renderTargetBlendDesc;
	DX11::Device()->CreateBlendState(&blendDesc, &m_BlendStates[BlendMode::Opaque]);

	// sampler states
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX11::Device()->CreateSamplerState(&samplerDesc, &m_SamplerStates[SamplerMode::Wrap]);
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	DX11::Device()->CreateSamplerState(&samplerDesc, &m_SamplerStates[SamplerMode::Point]);

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	DX11::Device()->CreateSamplerState(&samplerDesc, &m_SamplerStates[SamplerMode::Clamp]);

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	DX11::Device()->CreateSamplerState(&samplerDesc, &m_SamplerStates[SamplerMode::Border]);

	// depth stencil states
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = false;
	DX11::Device()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilStates[DepthStencilMode::ReadWrite]);

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DX11::Device()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilStates[DepthStencilMode::ReadOnly]);

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	DX11::Device()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilStates[DepthStencilMode::None]);
}

void Alpine::RenderStateManager::PushRasterizerState(CullMode cullMode)
{
	m_RasterizerStateStack.push(m_RasterizerStates[cullMode]);
	UpdateRasterizerState();
}

void Alpine::RenderStateManager::PopRasterizerState()
{
	m_RasterizerStateStack.pop();
	UpdateRasterizerState();
}

void Alpine::RenderStateManager::PushBlendState(BlendMode blendMode)
{
	m_BlendStateStack.push(m_BlendStates[blendMode]);
	UpdateBlendState();
}

void Alpine::RenderStateManager::PopBlendState()
{
	m_BlendStateStack.pop();
	UpdateBlendState();
}

void Alpine::RenderStateManager::PushDepthStencilState(DepthStencilMode depthStencilMode)
{
	m_DepthStencilStateStack.push(m_DepthStencilStates[depthStencilMode]);
	UpdateDepthStencilState();
}

void Alpine::RenderStateManager::PopDepthStencilState()
{
	m_DepthStencilStateStack.pop();
	UpdateDepthStencilState();
}

void Alpine::RenderStateManager::SetSamplerState(SamplerMode samplerMode, ShaderType shaderType, uint8_t slot)
{
	switch (shaderType)
	{
	case ShaderType::Vertex:
		DX11::Context()->VSSetSamplers(slot, 1, m_SamplerStates[samplerMode].GetAddressOf());
		break;
	case ShaderType::Pixel:
		DX11::Context()->PSSetSamplers(slot, 1, m_SamplerStates[samplerMode].GetAddressOf());
		break;
	case ShaderType::Compute:
		DX11::Context()->CSSetSamplers(slot, 1, m_SamplerStates[samplerMode].GetAddressOf());
		break;
	default:;
	}
}


void Alpine::RenderStateManager::UpdateRasterizerState()
{
	DX11::Context()->RSSetState(m_RasterizerStateStack.top().Get());
}

void Alpine::RenderStateManager::UpdateBlendState()
{
	DX11::Context()->OMSetBlendState(m_BlendStateStack.top().Get(), nullptr, 0xFFFFFFFF);
}

void Alpine::RenderStateManager::UpdateDepthStencilState()
{
	DX11::Context()->OMSetDepthStencilState(m_DepthStencilStateStack.top().Get(), 0);
}
