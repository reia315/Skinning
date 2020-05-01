#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh(Mesh * mesh, Skeleton * skelton, Animation * animation)
	: m_mesh(mesh)
	, m_skeleton(skelton)
	, m_animation(animation)
{
}

void SkinnedMesh::calculate(const Matrix4 & world, float frameNo)
{
	m_skeleton->calculateLocalMatrices(*m_animation, frameNo, m_localMatrices);
	m_skeleton->calculateWorldMatrices(world, m_localMatrices, m_worldMatrices);
	m_skeleton->calculateSkinnedMatrices(m_worldMatrices, m_skinnedMatrices);
}

void SkinnedMesh::draw(SkinnedMesh::Shader & shader) const
{
	shader.boneMatrices(m_skeleton->size(), m_skinnedMatrices);
	m_mesh->draw(shader);
}