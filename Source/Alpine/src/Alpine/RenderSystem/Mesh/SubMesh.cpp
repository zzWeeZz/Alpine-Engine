#include "SubMesh.h"

Alpine::SubMesh::SubMesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices)
{
	m_VertexBuffer.Initialize(aVertices.data(), aVertices.size());
	m_IndexBuffer.Initalize(aIndices.data(), aIndices.size());
}

Alpine::SubMesh::SubMesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices, Ref<Material> aMaterial)
{
	m_VertexBuffer.Initialize(aVertices.data(), aVertices.size());
	m_IndexBuffer.Initalize(aIndices.data(), aIndices.size());
	m_Material = Material::Create("sadf");
	m_Material = std::move(aMaterial);
}

Alpine::SubMesh::SubMesh(const SubMesh& aMesh) : m_IndexBuffer(aMesh.m_IndexBuffer)
{
	m_VertexBuffer = aMesh.m_VertexBuffer;
}

void Alpine::SubMesh::Draw()
{
	UINT offset = 0;
	if(m_Material) m_Material->Bind();
	DX11::Context()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), m_VertexBuffer.StridePtr(), &offset);
	DX11::Context()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	DX11::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Context()->DrawIndexed(m_IndexBuffer.BufferSize(), 0, 0);
}
