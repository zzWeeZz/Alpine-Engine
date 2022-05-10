#pragma once
#include "Alpine/DX11/Utilities.h"
#include "Alpine/DX11/RenderStateManager.h"

#include "Alpine/RenderSystem/Mesh/Mesh.h"

#include "Alpine/Assets/Materials/Material.h"

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