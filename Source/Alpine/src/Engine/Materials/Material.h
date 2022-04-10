#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Textures/Texture.h"

namespace Alpine
{
	class Material
	{
	public:
		Material(std::string name);
		void AddTexture(std::shared_ptr<Texture> texture);

		void Bind();

		static std::shared_ptr<Material> Create(std::string name);
	private:
		std::string myMaterialName;
		std::vector<std::shared_ptr<Texture>> myTextures;
	};

}
