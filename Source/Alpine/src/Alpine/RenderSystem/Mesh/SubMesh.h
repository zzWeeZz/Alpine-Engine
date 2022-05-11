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
#include "Alpine/Assets/Materials/Material.h"
#include <vector>
#include "Alpine/DX11/DX11.h"

namespace Alpine
{
	class SubMesh
	{
	public:
		SubMesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices);
		SubMesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices, Ref<Material> aMaterial);
		SubMesh(const SubMesh& aMesh);
		void Draw();
	private:
		Ref<Material> m_Material;
		VertexBuffer<Vertex> m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
	};
}