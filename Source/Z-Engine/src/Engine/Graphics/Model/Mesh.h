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

class Mesh
{
public:
	Mesh(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices);
	Mesh(const Mesh& aMesh);
	void Draw();
private:
	VertexBuffer<Vertex> myVertexBuffer;
	IndexBuffer myIndexBuffer;
	ID3D11DeviceContext* myDeviceContext;
};