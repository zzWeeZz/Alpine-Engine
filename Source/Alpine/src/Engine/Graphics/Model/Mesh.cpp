#include "Mesh.h"

Alpine::Mesh::Mesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices)
{
	myVertexBuffer.Initialize(aVertices.data(), aVertices.size());
	myIndexBuffer.Initalize(aIndices.data(), aIndices.size());
}

Alpine::Mesh::Mesh(const Mesh& aMesh)
{
	myIndexBuffer = aMesh.myIndexBuffer;
	myVertexBuffer = aMesh.myVertexBuffer;
}

void Alpine::Mesh::Draw()
{
	UINT offset = 0;
	DX11::GetDeviceContext()->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.StridePtr(), &offset);
	DX11::GetDeviceContext()->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	DX11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::GetDeviceContext()->DrawIndexed(myIndexBuffer.BufferSize(), 0, 0);
}
