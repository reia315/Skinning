#include "Animation.h"

#include <fstream>
#include <algorithm>

inline std::pair<int, int> findKeyFrameSegment(const std::vector<Animation::KeyFrame>& keys, float frameNo);

Animation::KeyFrame Animation::getKeyFrame(const std::string & boneName, float frameNo) const
{
	const auto keys = m_boneKeyFrames.find(boneName);
	KeyFrame result;

	if (keys == m_boneKeyFrames.end())
	{
		result.frameNo = frameNo;
		result.scale = Vector3(1.0f, 1.0f, 1.0f);
		result.rotate = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		result.position = Vector3(0.0f, 0.0f, 0.0f);
		return result;
	}

	const auto& segment = findKeyFrameSegment(keys->second, frameNo);
	if (segment.first == segment.second)
	{
		return keys->second[segment.first];
	}
	const auto& first = keys->second[segment.first];
	const auto& second = keys->second[segment.second];
	const float t = (frameNo - first.frameNo) / (second.frameNo - first.frameNo);
	result.frameNo = frameNo;

	result.scale = Vector3::lerp(first.scale, second.scale, t);
	result.rotate = Quaternion::slerp(first.rotate, second.rotate, t);
	result.position = Vector3::lerp(first.position, second.position, t);

	return result;
}

Animation::Animation()
{
}

void Animation::clear()
{
	m_boneKeyFrames.clear();
}

void Animation::load(const std::string & fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	if (!file)
	{
		throw std::runtime_error("can not open" + fileName);
	}

	clear();

	unsigned int boneFrameSize = 0;
	file.read(reinterpret_cast<char*>(&boneFrameSize), sizeof(boneFrameSize));

	for (unsigned int i = 0u; i < boneFrameSize; ++i)
	{
		char boneName[32];
		file.read(boneName, sizeof(boneName));
		unsigned int keyFrameSize = 0;
		file.read(reinterpret_cast<char*>(&keyFrameSize), sizeof(keyFrameSize));
		m_boneKeyFrames[boneName].resize(keyFrameSize);
		file.read(reinterpret_cast<char*>(m_boneKeyFrames.at(boneName).data()), sizeof(KeyFrame) * keyFrameSize);
	}
}

float Animation::endFrame() const
{
	float result = 0.0f;
	for (const auto& keys : m_boneKeyFrames)
	{
		result = std::max(result, keys.second.back().frameNo);
	}
	return result;
}

Matrix4 Animation::KeyFrame::matrix() const
{
	return Matrix4::createTRS(position, rotate, scale);
}

inline std::pair<int, int> findKeyFrameSegment(const std::vector<Animation::KeyFrame>& keys, float frameNo)
{
	int start = 0;
	int end = static_cast<int>(keys.size() - 1);

	if (keys[start].frameNo >= frameNo)
	{
		return std::pair<int, int>(start, start);
	}

	if (keys[end].frameNo <= frameNo)
	{
		return std::pair<int, int>(end, end);
	}

	while ((start + 1) < end)
	{
		const int mid = (start + end) / 2;

		if (keys[mid].frameNo == frameNo)
		{
			return std::pair<int, int>(mid, mid);
		}
		else if (keys[mid].frameNo < frameNo)
		{
			start = mid;
		}
		else
		{
			end = mid;
		}
	}
	return std::pair<int, int>(start, end);
}
