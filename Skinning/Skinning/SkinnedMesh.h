#pragma once

#include "Mesh.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Matrix4.h"

class SkinnedMesh
{
public:

	class Shader
		: public Mesh::Shader
	{
	public:

		virtual ~Shader() {};

		virtual void boneMatrices(int size, const Matrix4 matrices[]) = 0;
	};

public:

	SkinnedMesh(Mesh* mesh, Skeleton* skelton, Animation* animation);

	void calculate(const Matrix4& world, float frameNo);

	void draw(SkinnedMesh::Shader& shader) const;

private:

	static const unsigned int BONE_MAX{ 256 };

	const Mesh* m_mesh{ nullptr };
	const Skeleton* m_skeleton{ nullptr };
	const Animation* m_animation{ nullptr };
	Matrix4 m_localMatrices[BONE_MAX];
	Matrix4 m_worldMatrices[BONE_MAX];
	Matrix4 m_skinnedMatrices[BONE_MAX];
};