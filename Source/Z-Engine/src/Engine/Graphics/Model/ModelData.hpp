#pragma once
#include <d3d11.h>
#include "DirectXMath.h"

struct Vertex 
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
};