#include "Skeleton.h"

#include "Animation.h"
#include <fstream>

void Skeleton::load(const std::string & fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	if (!file)
	{
		throw std::runtime_error("can not open" + fileName);
	}
	clear();

	unsigned int skeltonSize = 0;
	file.read(reinterpret_cast<char*>(&skeltonSize), sizeof(skeltonSize));
	m_bones.resize(skeltonSize);
	file.read(reinterpret_cast<char*>(m_bones.data()), sizeof(Bone) * skeltonSize);
}

void Skeleton::calculateLocalMatrices(const Animation & animation, float frameNo, Matrix4 local[]) const
{
	for (const auto& bone : m_bones)
	{
		local[bone.id] = animation.getKeyFrame(bone.name, frameNo).matrix();
	}
}

void Skeleton::calculateWorldMatrices(const Matrix4 & parent, const Matrix4 local[], Matrix4 world[]) const
{
	world[0] = local[0] * m_bones[0].transform * parent;
	for (int i = 1; i < static_cast<int>(m_bones.size()); ++i)
	{
		world[i] = local[i] * m_bones[i].transform * world[m_bones[i].parentId];
	}
}

void Skeleton::calculateSkinnedMatrices(const Matrix4 world[], Matrix4 skinned[]) const
{
	for (const auto& bone : m_bones)
	{
		skinned[bone.id] = bone.inverse * world[bone.id];
	}
}

void Skeleton::clear()
{
	m_bones.clear();
}

std::size_t Skeleton::size() const
{
	return m_bones.size();
}
