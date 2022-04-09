#include "Material.h"

#include <spdlog/spdlog.h>

#include "DX11/DX11.h"

Alpine::Material::Material(std::string name)
{
	myMaterialName = name;
}

void Alpine::Material::PushTexture(const Texture& texture)
{
	if (myTextures.size() < 6)
	{
		myTextures.push_back(texture);
	}
	else
	{
		spdlog::critical("Material should not have more than 5 textures!");
		assert(false);
	}
}

void Alpine::Material::Bind()
{
	for (size_t i = 0; i < myTextures.size(); i++)
	{
		myTextures[i].Bind(static_cast<int>(i));
	}
}

std::shared_ptr<Alpine::Material> Alpine::Material::Create(std::string name)
{
	return std::make_shared<Material>(Material(name));
}
