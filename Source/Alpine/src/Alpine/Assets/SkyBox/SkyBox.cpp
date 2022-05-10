#include "SkyBox.h"

#include <imgui.h>


Alpine::SkyBox::SkyBox(const std::filesystem::path& skyBoxTexturePath) : m_SpecBuffer(4)
{
	m_IrrComputeShader = ComputeShader::Create("Shaders/IrradianceMap_cs.cso");
	m_SpecularComputeShader = ComputeShader::Create("Shaders/SpeclularMap_cs.cso");
	m_SpbrdfShader = ComputeShader::Create("Shaders/spdrdfGenerator_cs.cso");
	m_EquirectToCubeMapShader = ComputeShader::Create("Shaders/Equirect2Cube_cs.cso");
	ConvertToCubeMap(skyBoxTexturePath);
	FilterEnviorment();
	GenerateIrradience();
	GenerateSPBRDF();
	Ref<Material> skyBoxMaterial = Material::Create("skybox");
	skyBoxMaterial->AddTexture(m_CubeMap);
	m_Model = Model::Create("Cube", skyBoxMaterial);
}

void Alpine::SkyBox::Bind()
{
	m_SDRBF->Bind(6);
	m_IrMap->Bind(11);
	m_CubeMap->Bind(10);
}

void Alpine::SkyBox::BindForSky()
{
	m_CubeMap->Bind(0);
}


void Alpine::SkyBox::Draw()
{
	m_CubeMap->Bind(0);
	DX11::GetRenderStateManager().PushRasterizerState(CullMode::Front);
	DX11::GetRenderStateManager().PushDepthStencilState(DepthStencilMode::ReadOnly);
	m_Model->Draw(CullMode::Front, DepthStencilMode::ReadOnly);
	DX11::GetRenderStateManager().PopRasterizerState();
	DX11::GetRenderStateManager().PopDepthStencilState();
}

Alpine::Ref<Alpine::SkyBox> Alpine::SkyBox::Create(const std::filesystem::path& skyBoxTexturePath)
{
	return std::make_shared<SkyBox>(skyBoxTexturePath);
}

void Alpine::SkyBox::ConvertToCubeMap(const std::filesystem::path& skyBoxTexturePath)
{
	m_skyBoxTexture = Texture::Create(skyBoxTexturePath);
	m_CubeMap = TextureCube::Create(1024, 1024, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_CubeMap->CreateUAV();
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Compute);
	DX11::Context()->CSSetShaderResources(0, 1, m_skyBoxTexture->GetShaderResourceView().GetAddressOf());
	DX11::Context()->CSSetUnorderedAccessViews(0, 1, m_CubeMap->GetUnorderedAccessView().GetAddressOf(), 0);
	DX11::Context()->CSSetShader(m_EquirectToCubeMapShader->GetShader(), 0, 0);
	DX11::Context()->Dispatch(1024 / 32, 1024 / 32, 6);
	DX11::Context()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
	m_CubeMap->GenerateMipMaps();
}

void Alpine::SkyBox::FilterEnviorment()
{
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	m_SpecBuffer.Create();
	m_SpecularMap = TextureCube::Create(1024, 1024, DXGI_FORMAT_R16G16B16A16_FLOAT);
	// specular map
	DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Compute);
	DX11::Context()->CSSetShaderResources(0, 1, m_CubeMap->GetShaderResourceView().GetAddressOf());
	DX11::Context()->CSSetShader(m_SpecularComputeShader->GetShader(), 0, 0);

	for (uint32_t i = 0; i < 6; ++i)
	{
		uint32_t subResourceIndex = D3D11CalcSubresource(0, i, m_SpecularMap->GetLevels());
		DX11::Context()->CopySubresourceRegion(m_SpecularMap->GetTextures().Get(), subResourceIndex, 0, 0, 0, m_CubeMap->GetTextures().Get(), subResourceIndex, nullptr);
	}

	m_CubeMap->Bind(0, true);
	const float deltaRoughness = 1.0f / std::max((float)(m_SpecularMap->GetLevels() - 1), 1.0f);
	for (UINT level = 1, size = 512; level < m_SpecularMap->GetLevels(); ++level, size /= 2)
	{
		const UINT numGroups = std::max<UINT>(1, size / 32);
		m_SpecularMap->CreateUAV(level);
		const SpectularMapFillerSettingsBuffer al = { level * deltaRoughness };
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Compute);
		DX11::Context()->UpdateSubresource(m_SpecBuffer.GetBuffer(), 0, nullptr, &al, 0, 0);
		m_SpecBuffer.Bind(true, 0);
		DX11::Context()->CSSetUnorderedAccessViews(0, 1, m_SpecularMap->GetUnorderedAccessView().GetAddressOf(), nullptr);
		DX11::Context()->Dispatch(numGroups, numGroups, 6);
	}
	DX11::Context()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
}

void Alpine::SkyBox::GenerateIrradience()
{
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	m_IrMap = TextureCube::Create(32, 32, DXGI_FORMAT_R16G16B16A16_FLOAT, 1);
	m_IrMap->CreateUAV();
	m_CubeMap->Bind(9, true);
	DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Compute);
	DX11::Context()->CSSetShaderResources(0, 1, m_SpecularMap->GetShaderResourceView().GetAddressOf());
	DX11::Context()->CSSetUnorderedAccessViews(0, 1, m_IrMap->GetUnorderedAccessView().GetAddressOf(), 0);
	DX11::Context()->CSSetShader(m_IrrComputeShader->GetShader(), 0, 0);
	DX11::Context()->Dispatch(1, 1, 6);
	DX11::Context()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
}

void Alpine::SkyBox::GenerateSPBRDF(uint32_t width, uint32_t height)
{
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Clamp, ShaderType::Compute);
	m_SDRBF = Texture::Create(width, height, DXGI_FORMAT_R16G16_FLOAT, 1);
	m_SDRBF->CreateUAV();
	DX11::Context()->CSSetUnorderedAccessViews(0, 1, m_SDRBF->GetUnorderedAccessView().GetAddressOf(), 0);
	DX11::Context()->CSSetShader(m_SpbrdfShader->GetShader(), 0, 0);
	DX11::Context()->Dispatch(width / 32, height / 32, 1);
	DX11::Context()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
}
