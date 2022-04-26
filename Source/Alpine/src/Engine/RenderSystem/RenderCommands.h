#pragma once
#include "Engine/Graphics/Model/Mesh.h"
#include "Engine/Materials/Material.h"
#include "DX11/Utilities.h"
namespace Alpine
{
	struct MeshCommand
	{
		Ref<Material> material;
		Matrix transform;
		Mesh mesh;
	};
}