#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4.h"

class Animation
{
public:

	struct KeyFrame
	{
		float frameNo;
		Vector3 scale;
		Quaternion rotate;
		Vector3 position;
		Matrix4 matrix() const;
	};

public:

	Animation();

	KeyFrame getKeyFrame(const std::string& boneName, float frameNo) const;

	void clear();

	void load(const std::string& fileName);

	float endFrame() const;

private:

	std::unordered_map<std::string, std::vector<KeyFrame>> m_boneKeyFrames;
};