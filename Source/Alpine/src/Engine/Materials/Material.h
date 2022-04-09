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
		void PushTexture(const Texture& texture);

		void Bind();

		static std::shared_ptr<Material> Create(std::string name);
	private:
		std::string myMaterialName;
		std::vector<Texture> myTextures;
	};

}
