#pragma once

#include <string>
#include <vector>
#include "Matrix4.h"

class Animation;

class Skeleton
{
public:

	struct Bone
	{
		char		name[32];
		int			id;
		int			parentId;
		Matrix4	transform;
		Matrix4	inverse;
	};

public:

	Skeleton() = default;

	std::size_t size() const;

	void calculateLocalMatrices(const Animation& animation, float frameNo, Matrix4 local[]) const;

	void calculateWorldMatrices(const Matrix4& parent, const Matrix4 local[], Matrix4 world[]) const;

	void calculateSkinnedMatrices(const Matrix4 world[], Matrix4 skinned[]) const;

	void clear();

	void load(const std::string& fileName);

private:

	std::vector<Bone> m_bones;

};