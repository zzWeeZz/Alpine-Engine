#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Engine/Textures/Texture.h"
#include "Engine/Textures/TextureCube.h"

namespace Alpine
{
	class Material
	{
	public:
		Material(std::string name);
		void AddTexture(std::shared_ptr<Texture> texture);
		void AddTexture(std::shared_ptr<TextureCube> texture);

		void Bind();

		static std::shared_ptr<Material> Create(std::string name);
	private:
		std::string myMaterialName;
		Ref<TextureCube> m_Cube;
		std::vector<std::shared_ptr<Texture>> myTextures;
	};

}
