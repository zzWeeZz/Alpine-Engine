#pragma once
#include "Engine/Graphics/Model/Mesh.h"
#include "Engine/Materials/Material.h"
#include "DX11/Utilities.h"
#include "DX11/RenderStateManager.h"
namespace Alpine
{
	struct MeshCommand
	{
		Ref<Material> material;
		Matrix transform;
		Mesh mesh;
		CullMode cullmode = CullMode::Back;
		DepthStencilMode depthStencilMode = DepthStencilMode::ReadWrite;
	};
}