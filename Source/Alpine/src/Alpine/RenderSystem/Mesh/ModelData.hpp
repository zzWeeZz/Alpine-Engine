#pragma once
#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>
using namespace DirectX::SimpleMath;

struct Vertex 
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}
	Vector3 pos;
	Vector2 texCoord;
	Vector3 norm;
	Vector3 tang;
	Vector3 biTang;
};