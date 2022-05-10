#include "ShaderLibrary.h"

std::unordered_map<std::string, std::vector<Alpine::Ref<Alpine::Shader>>> Alpine::ShaderLibrary::s_Library;
void Alpine::ShaderLibrary::Store(std::string key, std::vector<Ref<Shader>> shaders)
{
	s_Library.insert(std::make_pair(key, shaders));
}

void Alpine::ShaderLibrary::Bind(std::string key)
{
	for (Ref<Shader> shader : s_Library[key])
	{
		shader->Bind();
	}
}