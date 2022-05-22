#pragma once
#include "Alpine/DX11/Utilities.h"
#include "Alpine/Alpine.h"

struct TagComponent
{
	std::string Tag;
};

struct TransformComponent
{
	Vector3 position = {0,0,0};
	Vector3 rotation = {0,0,0};
	Vector3 size = {1,1,1};
};

struct MeshComponent
{
	std::filesystem::path MeshPath;
	Alpine::Ref<Alpine::Model> model;
};