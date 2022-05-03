#pragma once
#include "Engine/Graphics/Shaders.h"
#include "Engine/Graphics/Model/Model.h"
#include "Engine/Graphics/Buffers/BufferData.h"
#include "Engine/Graphics/Buffers/ConstantBuffer.hpp"

#include "Engine/DX11/Utilities.h"

#include "Engine/Textures/Texture.h"
#include "Engine/Textures/TextureCube.h"

namespace Alpine
{
	class SkyBox
	{
	public:
		SkyBox(const std::filesystem::path& skyBoxTexturePath);

		void Bind();
		void BindForSky();
		void Draw();
		static Ref<SkyBox> Create(const std::filesystem::path& skyBoxTexturePath);
	private:
		void ConvertToCubeMap(const std::filesystem::path& skyBoxTexturePath);
		void FilterEnviorment();
		void GenerateIrradience();
		void GenerateSPBRDF(uint32_t width = 256, uint32_t height = 256);

		Ref<Model> m_Model;

		Ref<Texture> m_skyBoxTexture;
		Ref<Texture> m_SDRBF;
		Ref<TextureCube> m_CubeMap;
		Ref<TextureCube> m_SpecularMap;
		Ref<TextureCube> m_IrMap;

		Ref<ComputeShader> m_IrrComputeShader;
		Ref<ComputeShader> m_SpecularComputeShader;
		Ref<ComputeShader> m_SpbrdfShader;
		Ref<ComputeShader> m_EquirectToCubeMapShader;

		ConstantBuffer<SpectularMapFillerSettingsBuffer> m_SpecBuffer;
		SpectularMapFillerSettingsBuffer m_SpecBufferData;
	};
}