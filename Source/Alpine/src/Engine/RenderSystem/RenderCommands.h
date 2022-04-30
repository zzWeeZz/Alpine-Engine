#pragma once
#include "Engine/Graphics/Model/Mesh.h"
#include "Engine/Materials/Material.h"
#include "DX11/Utilities.h"
namespace Alpine
{
	struct MeshCommand
	{
		MeshCommand(Ref<Material> mat, Matrix trans, Mesh me, bool usePbr)
		{
			material = mat;
			transform = trans;
			mesh = me;
			usePBR = usePbr;
		}
		Ref<Material> material;
		Matrix transform;
		Mesh mesh;
		bool usePBR = true;
	};
}