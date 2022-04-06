#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXTK/WICTextureLoader.h>
#include <string>
#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexBuffer.h"
#include "ModelData.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include "DX11/DX11.h"

namespace Alpine
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices);
		Mesh(const Mesh& aMesh);
		void Draw();
	private:
		VertexBuffer<Vertex> myVertexBuffer;
		IndexBuffer myIndexBuffer;
	};
}