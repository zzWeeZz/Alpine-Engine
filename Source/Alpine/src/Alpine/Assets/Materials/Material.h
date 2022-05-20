#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Alpine/Assets/Textures/Texture.h"
#include "Alpine/Assets/Textures/TextureCube.h"

namespace Alpine
{
	class Material
	{
	public:
		Material(std::string name);
		Material(const Material& other);
		void AddDiffuseTexture(Ref<Texture> texture);
		void AddNormalTexture(Ref<Texture> texture);
		void AddSpecularTexture(Ref<Texture> texture);

		void AddTexture(Ref<TextureCube> texture);

		void Bind();

		static std::shared_ptr<Material> Create(std::string name);
	private:
		std::string myMaterialName;
		Ref<TextureCube> m_Cube;
		std::vector<Ref<Texture>> myTextures;
	};

}
