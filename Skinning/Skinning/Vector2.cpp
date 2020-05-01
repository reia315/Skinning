#include "Vector2.h"



#include <cmath>

#include <limits>

#include <algorithm>

#include <stdexcept>



#include "MathHelper.h"

#include "Matrix3.h"



const Vector2 Vector2::zero(0.0f, 0.0f);

const Vector2 Vector2::one(1.0f, 1.0f);

const Vector2 Vector2::up(0.0f, 1.0f);

const Vector2 Vector2::down(0.0f, -1.0f);

const Vector2 Vector2::left(-1.0f, 0.0f);

const Vector2 Vector2::right(1.0f, 0.0f);

const Vector2 Vector2::negativeInfinity(-INFINITY, -INFINITY);

const Vector2 Vector2::positiveInfinity(INFINITY, INFINITY);



Vector2::Vector2(float x, float y)

	: x(x)

	, y(y)

{

}



Vector2::Vector2(float value)

	: x(value)

	, y(value)

{

}



float Vector2::operator[](int index) const

{

	switch (index)

	{

	case 0: return x; break;

	case 1: return y; break;

	}

	throw std::runtime_error("vector2 operator[] out of range");

}



float & Vector2::operator[](int index)

{

	switch (index)

	{

	case 0: return x; break;

	case 1: return y; break;

	}

	throw std::runtime_error("vector2 operator[] out of range");

}



Vector2 Vector2::normalize(const Vector2 & value)

{

	return Vector2(value).normalized();

}



float Vector2::angle(const Vector2 & vector1, const Vector2 & vector2)

{

	float denominator = std::sqrt(vector1.lengthSquared() * vector2.lengthSquared());



	if (denominator == 0.0f) return 0.0f;



	float dotValue = MathHelper::clamp(dot(vector1, vector2) / denominator, -1.0f, 1.0f);



	return MathHelper::acos(dotValue);

}



float Vector2::signedAngle(const Vector2 & vector1, const Vector2 & vector2)

{

	float unSignedAngle = angle(vector1, vector2);



	// TODO : žB–†‚È•Ï”–¼‚ð’¼‚·

	float value = vector1.x * vector2.y - vector1.y * vector2.x;



	return unSignedAngle * MathHelper::sign(value);

}



float Vector2::dot(const Vector2 & vector1, const Vector2 & vector2)

{

	return (vector1.x * vector2.x) + (vector1.y * vector2.y);

}



float Vector2::cross(const Vector2 & vector1, const Vector2 & vector2)

{

	return  (vector1.x * vector2.y) - (vector1.y * vector2.x);

}



float Vector2::distance(const Vector2 & vector1, const Vector2 & vector2)

{

	return (vector2 - vector1).length();

}



float Vector2::distanceSquared(const Vector2 & vector1, const Vector2 & vector2)

{

	return (vector2 - vector1).lengthSquared();

}



Vector2 Vector2::lerp(const Vector2 & value1, const Vector2 & value2, float amount)

{

	return Vector2(MathHelper::lerp(value1.x, value2.x, amount), MathHelper::lerp(value1.y, value2.y, amount));

}



Vector2 Vector2::lerpUnclamped(const Vector2 & value1, const Vector2 & value2, float amount)

{

	return Vector2(MathHelper::lerpUnclamped(value1.x, value2.x, amount), MathHelper::lerpUnclamped(value1.y, value2.y, amount));;

}



Vector2 Vector2::moveTowards(const Vector2 & current, const Vector2 & target, float maxDistanceDelta)

{

	float toVectorX = target.x - current.x;

	float toVectorY = target.y - current.y;



	float sqDist = toVectorX * toVectorX * toVectorY * toVectorY;



	if (sqDist == 0 || (maxDistanceDelta >= 0 && sqDist <= maxDistanceDelta * maxDistanceDelta))

		return target;



	float dist = std::sqrt(sqDist);



	return Vector2(

		current.x + toVectorX / dist * maxDistanceDelta,

		current.y + toVectorY / dist * maxDistanceDelta

	);

}



Vector2 Vector2::smoothDamp(const Vector2 & current, const Vector2 & target, Vector2 * currentVelocity, float smoothTime, float maxSpeed, float deltaTime)

{

	smoothTime = std::fmax(0.0001f, smoothTime);

	float omega = 2.0f / smoothTime;



	float x = omega * deltaTime;

	// TODO : “ä‚Ì”Žš‚É“ä‚ÌŽ®

	float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);



	float changeX = current.x - target.x;

	float changeY = current.y - target.y;



	float maxChange = maxSpeed * smoothTime;



	float maxChangeSq = maxChange * maxChange;

	float sqDist = changeX * changeX + changeY * changeY;

	if (sqDist > maxChangeSq)

	{

		float mag = sqrt(sqDist);

		changeX = changeX / mag * maxChange;

		changeY = changeY / mag * maxChange;

	}



	Vector2 afterCalcTarget = target;



	afterCalcTarget.x = current.x - changeX;

	afterCalcTarget.y = current.y - changeY;



	float tempX = (currentVelocity->x + omega * changeX) * deltaTime;

	float tempY = (currentVelocity->y + omega * changeY) * deltaTime;



	currentVelocity->x = (currentVelocity->x - omega * tempX) * exp;

	currentVelocity->y = (currentVelocity->y - omega * tempY) * exp;



	float outputX = afterCalcTarget.x + (changeX + tempX) * exp;

	float outputY = afterCalcTarget.y + (changeY + tempY) * exp;



	float origMinusCurrentX = target.x - current.x;

	float origMinusCurrentY = target.y - current.y;



	float outMinusOrigX = outputX - target.x;

	float outMinusOrigY = outputY - target.y;



	if (origMinusCurrentX * outMinusOrigX + origMinusCurrentY * outMinusOrigY > 0.0f)

	{

		outputX = target.x;

		outputY = target.y;



		currentVelocity->x = (outputX - target.x) / deltaTime;

		currentVelocity->y = (outputY - target.y) / deltaTime;

	}

	return Vector2(outputX, outputY);

}



Vector2 Vector2::calculateMin(const Vector2 & vector1, const Vector2 & vector2)

{

	return Vector2(std::min(vector1.x, vector2.x), std::min(vector1.y, vector2.y));

}



Vector2 Vector2::calculateMax(const Vector2 & vector1, const Vector2 & vector2)

{

	return Vector2(std::max(vector1.x, vector2.x), std::min(vector1.y, vector2.y));

}



Vector2 Vector2::clamp(const Vector2 & value1, const Vector2& min, const Vector2 & max)

{

	return Vector2(MathHelper::clamp(value1.x, min.x, max.x), MathHelper::clamp(value1.y, min.y, max.y));

}



Vector2 Vector2::clampLength(const Vector2 & vector1, float maxLength)

{

	float sqrLength = vector1.lengthSquared();



	if (sqrLength > maxLength * maxLength)

	{

		float length = std::sqrt(sqrLength);



		float normalizedX = vector1.x / length;

		float normalizedY = vector1.y / length;



		return Vector2(normalizedX * maxLength, normalizedY * maxLength);

	}

	return vector1;

}



Vector2 Vector2::perpendicular(const Vector2 & inDirection)

{

	return Vector2(-inDirection.y, inDirection.x);

}



Vector2 Vector2::reflect(const Vector2 & inDirection, const Vector2 & inNormal)

{

	float factor = -2.0f * dot(inNormal, inDirection);



	return Vector2(factor * inNormal.x + inDirection.x, factor * inNormal.y + inDirection.y);

}



Vector2 Vector2::scale(const Vector2 & vector1, const Vector2 & vector2)

{

	return Vector2(vector1.x * vector2.x, vector1.y * vector2.y);

}



Vector2 Vector2::transform(const Vector2 & position, const Matrix3 & matrix)

{

	float w = position.x * matrix.m[0][2] + position.y * matrix.m[1][2] + matrix.m[2][2];



	return Vector2(

		(position.x * matrix.m[0][0] + position.y * matrix.m[1][0] + matrix.m[2][0]) / w,

		(position.x * matrix.m[0][1] + position.y * matrix.m[1][1] + matrix.m[2][1]) / w

	);

}



float Vector2::length() const

{

	return std::sqrt(dot(*this, *this));

}



float Vector2::lengthSquared() const

{

	return dot(*this, *this);

}



Vector2 Vector2::normalized() const

{

	float len = length();



	if (len > MathHelper::kEpsilon)

	{

		return Vector2(x / len, y / len);

	}

	return Vector2::zero;

}



bool Vector2::equals(const Vector2 & other)

{

	return (MathHelper::equals(x, other.x) && MathHelper::equals(y, other.y));

}



void Vector2::setValue(float x, float y)

{

	Vector2::x = x;

	Vector2::y = y;

}



std::string Vector2::toString() const

{

	return  "[x:" + std::to_string(x) + ", y:" + std::to_string(y) + "]";

}



Vector2 operator-(const Vector2 & v)

{

	return Vector2(-v.x, -v.y);

}



Vector2 & operator+=(Vector2 & v1, const Vector2 & v2)

{

	v1.x += v2.x;

	v1.y += v2.y;

	return v1;

}



Vector2 & operator-=(Vector2 & v1, const Vector2 & v2)

{

	v1.x -= v2.x;

	v1.y -= v2.y;

	return v1;

}



Vector2 & operator*=(Vector2 & v, float s)

{

	v.x *= s;

	v.y *= s;

	return v;

}



Vector2 & operator*=(Vector2 & v, const Matrix3 & m)

{

	v = v * m;

	return v;

}



Vector2 & operator/=(Vector2 & v, float s)

{

	v.x /= s;

	v.y /= s;

	return v;

}



Vector2 operator+(Vector2 v1, const Vector2 & v2)

{

	return v1 += v2;

}



Vector2 operator-(Vector2 v1, const Vector2 & v2)

{

	return v1 -= v2;

}



Vector2 operator*(Vector2 v, float s)

{

	return v *= s;

}



Vector2 operator*(float s, Vector2 v)

{

	return v *= s;

}



Vector2 operator*(const Vector2 & v, const Matrix3 & m)

{

	return Vector2::transform(v, m);

}



Vector2 operator/(Vector2 v, float s)

{

	return v /= s;

}



bool operator==(const Vector2 & lhs, const Vector2 & rhs)

{

	return ((lhs.x == rhs.x) && (lhs.y == rhs.y));

}



bool operator!=(const Vector2 & lhs, const Vector2 & rhs)

{

	return !(lhs == rhs);

}



std::ostream & operator<<(std::ostream & os, const Vector2 & vector2)

{

	return os << vector2.toString();

}