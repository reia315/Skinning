#include "Mesh.h"


#include <fstream>
#include <vector>
#include <cstddef>
#include <filesystem>
#include "lodepng.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_device{ device }
	, m_deviceContext{ deviceContext }
{

}

Mesh::~Mesh()
{
	clear();
}

void Mesh::load(const std::string& fileName)
{
	std::ifstream file(fileName, std::ios::binary);

	if (!file)
	{
		throw std::runtime_error("can not open" + fileName);
	}
	clear();

	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;

	{
		unsigned int materialSize = 0;
		file.read(reinterpret_cast<char*>(&materialSize), sizeof(materialSize));
		m_materials.resize(materialSize);
		file.read(reinterpret_cast<char*>(m_materials.data()), sizeof(Material) * materialSize);
	}

	{
		unsigned int subsetSize = 0;
		file.read(reinterpret_cast<char*>(&subsetSize), sizeof(subsetSize));
		m_subsets.resize(subsetSize);
		file.read(reinterpret_cast<char*>(m_subsets.data()), sizeof(Subset) * subsetSize);
	}

	{
		unsigned int indicesSize = 0;
		file.read(reinterpret_cast<char*>(&indicesSize), sizeof(indicesSize));
		indices.resize(indicesSize);
		file.read(reinterpret_cast<char*>(indices.data()), sizeof(unsigned int) * indicesSize);
	}

	{
		unsigned int verticesSize = 0;
		file.read(reinterpret_cast<char*>(&verticesSize), sizeof(verticesSize));
		vertices.resize(verticesSize);
		file.read(reinterpret_cast<char*>(vertices.data()), sizeof(Vertex) * verticesSize);
	}

	std::experimental::filesystem::path meshPath(fileName);

	std::string texturePath;

	if (meshPath.has_parent_path())
	{
		texturePath = meshPath.parent_path().string() + "/";
	}

	for (auto& mat : m_materials)
	{
		mat.texture = new Texture2D(m_device, m_deviceContext);
		mat.texture->load(texturePath + mat.textureFileName);
	}

	m_vertices = createBuffer(
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(Vertex) * vertices.size(),
		vertices.data()
	);

	m_indices = createBuffer(D3D11_BIND_INDEX_BUFFER, sizeof(unsigned int) * indices.size(), indices.data());
}

void Mesh::draw(Mesh::Shader& shader) const
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertices, &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indices, DXGI_FORMAT_R32_UINT, 0);

	for (auto&& subset : m_subsets)
	{
		shader.material(m_materials[subset.material]);
		shader.begin();

		m_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_deviceContext->DrawIndexed(subset.count, subset.start, 0);

		shader.end();
	}
}

void Mesh::clear()
{
	m_subsets.clear();

	for (auto&& mat : m_materials)
	{
		mat.texture->clear();
		delete(mat.texture);
	}
	m_materials.clear();

	if (m_indices != nullptr)
	{
		m_indices->Release();
		m_indices = nullptr;
	}

	if (m_vertices != nullptr)
	{
		m_vertices->Release();
		m_vertices = nullptr;
	}
}

ID3D11Buffer * Mesh::createBuffer(UINT bind, UINT size, const void * data)
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = bind;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresourceData{};
	subresourceData.pSysMem = data;

	ID3D11Buffer* buffer{};
	m_device->CreateBuffer(&desc, &subresourceData, &buffer);

	return buffer;
}

