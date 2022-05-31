#include "ShaderLibrary.h"

std::unordered_map<std::string, std::vector<Alpine::Ref<Alpine::Shader>>> Alpine::ShaderLibrary::s_Library;
std::string Alpine::ShaderLibrary::s_CurrentBoundKey;
void Alpine::ShaderLibrary::Store(std::string key, std::vector<Ref<Shader>> shaders)
{
	s_Library.insert(std::make_pair(key, shaders));
}

void Alpine::ShaderLibrary::Bind(std::string key)
{
	if (strcmp(s_CurrentBoundKey.c_str(), key.c_str()))
	{
		return;
	}

	for (Ref<Shader> shader : s_Library[key])
	{
		shader->Bind();
	}
	s_CurrentBoundKey = key;
}