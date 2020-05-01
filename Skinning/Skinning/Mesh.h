#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"
#include "Texture2D.h"
#include <vector>
#include <string>
#include <d3d11.h>

class Mesh
{
public:

	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		Color emission;
		float shiniess;
		char textureFileName[128];
		Texture2D* texture;
	};

	struct Subset
	{
		int material;
		int start;
		int count;
	};

	struct Vertex
	{
		Vector3 point;
		Vector3 normal;
		Vector2 texcoord;
		unsigned char bones[4];
		float weight[4];
	};

	class Shader
	{
	public:

		virtual ~Shader() = default;

		virtual void begin() = 0;

		virtual void end() = 0;

		virtual void material(const Mesh::Material& material) = 0;
	};

public:

	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	~Mesh();

	void load(const std::string& fileName);

	void draw(Mesh::Shader& shader) const;

	void clear();

	Mesh(const Mesh& other) = delete;
	Mesh& operator = (const Mesh& other) = delete;

private:

	ID3D11Buffer* createBuffer(UINT bind, UINT size, const void* data);

private:

	ID3D11Device* m_device{ nullptr };

	ID3D11DeviceContext* m_deviceContext{ nullptr };

	std::vector<Material> m_materials;

	std::vector<Subset> m_subsets;

	ID3D11Buffer* m_indices{ nullptr };

	ID3D11Buffer* m_vertices{ nullptr };
};