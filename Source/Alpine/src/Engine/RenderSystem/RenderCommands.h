#pragma once
#include "Engine/Graphics/Buffers/VertexBuffer.h"
#include "Engine/Graphics/Buffers/IndexBuffer.h"
#include "Engine/Graphics/Model/ModelData.hpp"
#include "Engine/Materials/Material.h"
namespace Alpine
{
	struct ModelCommand
	{
		VertexBuffer<Vertex> vertexBuffer;
		IndexBuffer indexBuffer;
		std::shared_ptr<Material> material;
	};
}