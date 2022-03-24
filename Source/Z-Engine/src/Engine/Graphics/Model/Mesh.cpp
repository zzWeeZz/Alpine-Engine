#include "Mesh.h"

Mesh::Mesh(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices)
{
	myDeviceContext = aDeviceContext;
	myVertexBuffer.Initialize(aDevice, aVertices.data(), aVertices.size());
	myIndexBuffer.Initalize(aDevice, aIndices.data(), aIndices.size());
}

Mesh::Mesh(const Mesh& aMesh)
{
	myDeviceContext = aMesh.myDeviceContext;
	myIndexBuffer = aMesh.myIndexBuffer;
	myVertexBuffer = aMesh.myVertexBuffer;
}

void Mesh::Draw(ID3D11RasterizerState* state)
{
	UINT offset = 0;
	myDeviceContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.StridePtr(), &offset);
	myDeviceContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	myDeviceContext->RSSetState(state);
	myDeviceContext->DrawIndexed(myIndexBuffer.BufferSize(), 0, 0);
}
