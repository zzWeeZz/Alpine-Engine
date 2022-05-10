#pragma once
#include "Shader.h"
#include "Alpine/DX11/Utilities.h"

#include <unordered_map>
#include <string>

namespace Alpine
{
class ShaderLibrary
{
public:
	static void Store(std::string key, std::vector<Ref<Shader>> shaders);
	static void Bind(std::string key);
private:
	static std::unordered_map<std::string, std::vector<Ref<Shader>>> s_Library;
};
}