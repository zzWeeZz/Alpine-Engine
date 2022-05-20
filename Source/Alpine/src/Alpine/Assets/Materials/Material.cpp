#include "Material.h"

#include <spdlog/spdlog.h>

#include "Alpine/DX11/DX11.h"

Alpine::Material::Material(std::string name)
{
	myMaterialName = name;
}

Alpine::Material::Material(const Material& other)
{
	myMaterialName = other.myMaterialName;
	myTextures = other.myTextures;
	myTextures.resize(3);
}

void Alpine::Material::AddDiffuseTexture(Ref<Texture> texture)
{
	myTextures[0] = texture;
}

void Alpine::Material::AddNormalTexture(Ref<Texture> texture)
{
	myTextures[1] = texture;
}

void Alpine::Material::AddSpecularTexture(Ref<Texture> texture)
{
	myTextures[2] = texture;
}


void Alpine::Material::AddTexture(std::shared_ptr<TextureCube> texture)
{
	m_Cube = texture;
}

void Alpine::Material::Bind()
{
	if(m_Cube)
	{
		m_Cube->Bind(0);
	}
	for (size_t i = 0; i < myTextures.size(); i++)
	{
		myTextures[i]->Bind(static_cast<int>(i));
	}
}

std::shared_ptr<Alpine::Material> Alpine::Material::Create(std::string name)
{
	return std::make_shared<Material>(Material(name));
}
