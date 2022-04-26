#pragma once
#include "Graphics/Shaders.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Buffers/BufferData.h"
#include "Graphics/Buffers/ConstantBuffer.hpp"

#include "DX11/Utilities.h"

#include "Textures/Texture.h"
#include "Textures/TextureCube.h"

namespace Alpine
{
	class SkyBox
	{
	public:
		SkyBox(const std::filesystem::path& skyBoxTexturePath);

		void Bind();
		void Draw();
		void Check();
		static Ref<SkyBox> Create(const std::filesystem::path& skyBoxTexturePath);
	private:
		void ConvertToCubeMap(const std::filesystem::path& skyBoxTexturePath);
		void FilterEnviorment();
		void GenerateIrradience();
		void GenerateSPBRDF(uint32_t width = 256, uint32_t height = 256);

		VertexShader m_VertexShader;
		PixelShader m_PixelShader;

		Model m_Model;


		Ref<Texture> m_skyBoxTexture;
		Ref<Texture> m_SDRBF;
		Ref<TextureCube> m_CubeMap;
		Ref<TextureCube> m_SpecularMap;
		Ref<TextureCube> m_IrMap;

		ComputeShader m_IrrComputeShader;
		ComputeShader m_SpecularComputeShader;
		ComputeShader m_SpbrdfShader;
		ComputeShader m_EquirectToCubeMapShader;

		ConstantBuffer<SpectularMapFilerSettingsBuffer> m_SpecBuffer;

		SpectularMapFilerSettingsBuffer m_SpecBufferData;
	};
}