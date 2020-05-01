#include "MathHelper.h"

#include <cmath>

#include <algorithm>





const float MathHelper::pi = 3.14159265359f;

const float MathHelper::kEpsilon = 1e-5f;



bool MathHelper::equals(float value1, float value2)

{

	return (MathHelper::distance(value1, value2) < MathHelper::kEpsilon);

}



float MathHelper::sign(float value)

{

	if (std::abs(value) < MathHelper::kEpsilon) return 1;



	return (value / std::abs(value));

}



float MathHelper::clamp(float value, float min, float max)

{

	return std::max(min, std::min(value, max));

}



float MathHelper::distance(float value1, float value2)

{

	return std::abs(value1 - value2);

}



// ※未デバック

float MathHelper::lerp(float value1, float value2, float amount)

{

	amount = clamp(amount, 0.0f, 1.0f);



	return lerpUnclamped(value1, value2, amount);

}



// ※未デバック

float MathHelper::lerpUnclamped(float value1, float value2, float amount)

{

	return (value1 * (1.0f - amount)) + (value2 * amount);

}



// ※未デバック

float MathHelper::moveTowards(float current, float target, float maxDistanceDelta)

{

	float toTarget = target - current;



	float sqdist = toTarget * toTarget;



	if (sqdist < kEpsilon || sqdist <= (maxDistanceDelta * maxDistanceDelta))

	{

		return target;

	}



	float dist = std::sqrt(sqdist);



	return current + toTarget / dist * maxDistanceDelta;

}



float MathHelper::toDegrees(float radian)

{

	return radian * (180.f / MathHelper::pi);

}



float MathHelper::toRadians(float degree)

{

	return degree * (MathHelper::pi / 180.0f);

}



float MathHelper::sin(float degree)

{

	return std::sin(toRadians(degree));

}



float MathHelper::cos(float degree)

{

	return std::cos(toRadians(degree));

}



float MathHelper::tan(float degree)

{

	return std::tan(toRadians(degree));

}



float MathHelper::asin(float s)

{

	return toDegrees(std::asin(s));

}



float MathHelper::acos(float c)

{

	return toDegrees(std::acos(c));

}



float MathHelper::atan(float y, float x)

{

	return toDegrees(std::atan2(y, x));

}
