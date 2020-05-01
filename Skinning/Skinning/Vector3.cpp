#include "Vector3.h"



#include <cmath>

#include <limits>

#include <algorithm>

#include <stdexcept>



#include "Matrix4.h"

#include "MathHelper.h"

#include "Quaternion.h"



const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);

const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);

const Vector3 Vector3::down(0.0f, -1.0f, 0.0f);

const Vector3 Vector3::left(-1.0f, 0.0f, 0.0f);

const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);

const Vector3 Vector3::forwardLH(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::backwardLH(0.0f, 0.0f, -1.0f);

const Vector3 Vector3::forwardRH(0.0f, 0.0f, -1.0f);

const Vector3 Vector3::backwardRH(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::negativeInfinity(-INFINITY, -INFINITY, -INFINITY);

const Vector3 Vector3::positiveInfinity(INFINITY, INFINITY, INFINITY);



Vector3::Vector3(float x, float y, float z)

	: x(x)

	, y(y)

	, z(z)

{

}



Vector3::Vector3(float value)

	: x(value)

	, y(value)

	, z(value)

{

}



float Vector3::operator[](int index) const

{

	switch (index)

	{

	case 0: return x; break;

	case 1: return y; break;

	case 2: return z; break;

	}

	throw std::runtime_error("vector3 operator[] out of range");

}



float & Vector3::operator[](int index)

{

	switch (index)

	{

	case 0: return x; break;

	case 1: return y; break;

	case 2: return z; break;

	}

	throw std::runtime_error("vector3 operator[] out of range");

}



Vector3 Vector3::normalize(const Vector3 & value)

{

	return Vector3(value).normalized();

}



void Vector3::orthoNormalize(Vector3 * normal, Vector3 * tangent, Vector3 * binormal)

{

	*normal = Vector3::normalize(*normal);



	if (normal->lengthSquared() <= (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		*normal = Vector3::right;

	}



	Vector3 bn = Vector3::cross(*normal, *tangent);



	if (bn.lengthSquared() <= (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		*tangent = Vector3::cross(Vector3::forwardLH, *normal);

	}



	if (tangent->lengthSquared() <= (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		*tangent = Vector3::cross(Vector3::right, *normal);

	}



	bn = Vector3::cross(*normal, *tangent);

	bn = Vector3::normalize(bn);



	*tangent = Vector3::cross(bn, *normal);

	*binormal = (Vector3::dot(bn, *binormal) < 0.0f) ? -bn : bn;

}



Vector3 Vector3::project(const Vector3 & vector, const Vector3 & onNormal)

{

	float sqrLen = onNormal.lengthSquared();



	if (sqrLen < MathHelper::kEpsilon) return Vector3::zero;



	float dotValue = dot(vector, onNormal);



	return Vector3(

		onNormal.x * dotValue / sqrLen,

		onNormal.y * dotValue / sqrLen,

		onNormal.z * dotValue / sqrLen

	);

}



Vector3 Vector3::projectOnPlane(const Vector3 & value, const Vector3 & planeNormal)

{

	float sqrLen = planeNormal.lengthSquared();



	if (sqrLen < MathHelper::kEpsilon) return value;



	float dotValue = dot(value, planeNormal);



	return Vector3(

		value.x - planeNormal.x * dotValue / sqrLen,

		value.y - planeNormal.y * dotValue / sqrLen,

		value.z - planeNormal.z * dotValue / sqrLen

	);

}



float Vector3::dot(const Vector3 & vector1, const Vector3 & vector2)

{

	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);

}



Vector3 Vector3::cross(const Vector3 & vector1, const Vector3 & vector2)

{

	return Vector3(

		vector1.y * vector2.z - vector1.z * vector2.y,

		vector1.z * vector2.x - vector1.x * vector2.z,

		vector1.x * vector2.y - vector1.y * vector2.x);

}



float Vector3::distance(const Vector3 & vector1, const Vector3 & vector2)

{

	return (vector2 - vector1).length();

}



float Vector3::distanceSquared(const Vector3 & vector1, const Vector3 & vector2)

{

	return 	(vector2 - vector1).lengthSquared();

}



Vector3 Vector3::lerp(const Vector3 & value1, const Vector3 & value2, float amount)

{

	return Vector3(

		MathHelper::lerp(value1.x, value2.x, amount),

		MathHelper::lerp(value1.y, value2.y, amount),

		MathHelper::lerp(value1.z, value2.z, amount));

}



Vector3 Vector3::lerpUnclamped(const Vector3 & value1, const Vector3 & value2, float amount)

{

	return Vector3(

		MathHelper::lerpUnclamped(value1.x, value2.x, amount),

		MathHelper::lerpUnclamped(value1.y, value2.y, amount),

		MathHelper::lerpUnclamped(value1.z, value2.z, amount)

	);

}



Vector3 Vector3::moveTowards(const Vector3 & current, const Vector3 & target, float maxDistanceDelta)

{

	float toVector_x = target.x - current.x;

	float toVector_y = target.y - current.y;

	float toVector_z = target.z - current.z;



	float sqdist = (toVector_x * toVector_x) + (toVector_y * toVector_y) + (toVector_z * toVector_z);



	if (sqdist == 0 || sqdist <= (maxDistanceDelta * maxDistanceDelta))

	{

		return target;

	}



	float dist = std::sqrt(sqdist);



	return Vector3(

		current.x + toVector_x / dist * maxDistanceDelta,

		current.y + toVector_y / dist * maxDistanceDelta,

		current.z + toVector_z / dist * maxDistanceDelta

	);

}



Vector3 Vector3::calculateMin(const Vector3 & vector1, const Vector3 & vector2)

{

	return Vector3(

		std::fmin(vector1.x, vector2.x),

		std::fmin(vector1.y, vector2.y),

		std::fmin(vector1.z, vector2.z));

}



Vector3 Vector3::calculateMax(const Vector3 & vector1, const Vector3 & vector2)

{

	return Vector3(

		std::fmax(vector1.x, vector2.x),

		std::fmax(vector1.y, vector2.y),

		std::fmax(vector1.z, vector2.z));

}



Vector3 Vector3::clamp(const Vector3 & value1, const Vector3 & min, const Vector3 & max)

{

	return Vector3(

		MathHelper::clamp(value1.x, min.x, max.x),

		MathHelper::clamp(value1.y, min.y, max.y),

		MathHelper::clamp(value1.z, min.z, max.z));

}



Vector3 Vector3::clampLength(const Vector3 & value, float maxLength)

{

	float sqrLen = value.lengthSquared();



	if (sqrLen < maxLength * maxLength)

	{

		float mag = std::sqrt(sqrLen);



		float normalized_x = value.x / mag;

		float normalized_y = value.y / mag;

		float normalized_z = value.z / mag;



		return Vector3(

			normalized_x * maxLength,

			normalized_y * maxLength,

			normalized_z * maxLength

		);

	}

	return value;

}



Vector3 Vector3::reflect(const Vector3 & inDirection, const Vector3 & inNormal)

{

	float factor = -2.0f * dot(inNormal, inDirection);



	return Vector3(

		factor * inNormal.x + inDirection.x,

		factor * inNormal.y + inDirection.y,

		factor * inNormal.z + inDirection.z

	);

}



Vector3 Vector3::scale(const Vector3 & vector1, const Vector3 & vector2)

{

	return Vector3(

		vector1.x * vector2.x,

		vector1.y * vector2.y,

		vector1.z * vector2.z

	);

}



float Vector3::signedAngle(const Vector3 & from, const Vector3 & to, const Vector3 & axis)

{

	float unsignedAngle = angle(from, to);



	float cross_x = from.y * to.z - from.z * to.y;

	float cross_y = from.z * to.x - from.x * to.z;

	float cross_z = from.x * to.y - from.y * to.x;



	float sign = MathHelper::sign(axis.x * cross_x + axis.y * cross_y + axis.z * cross_z);



	return unsignedAngle * sign;

}



Vector3 Vector3::rotateTowards(const Vector3 & current, const Vector3 & target, float maxRadiansDelta, float maxMagnitudeDelta)

{

	float curLength = current.length();

	float targetLength = target.length();



	if (curLength * targetLength < MathHelper::kEpsilon)

	{

		return Vector3::moveTowards(current, target, maxMagnitudeDelta);

	}



	Vector3 afterNormalizeCurrent = current / curLength;

	Vector3 afterNormalizeTarget = target / targetLength;



	float angleRad = MathHelper::toRadians(Vector3::angle(afterNormalizeCurrent, afterNormalizeTarget));



	Vector3 axis = Vector3::cross(afterNormalizeCurrent, afterNormalizeTarget);



	if (axis.lengthSquared() < (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		axis = Vector3::cross(Vector3::forwardLH, afterNormalizeCurrent);



		if (axis.lengthSquared() < (MathHelper::kEpsilon * MathHelper::kEpsilon))

		{

			axis = Vector3::cross(Vector3::right, afterNormalizeCurrent);

		}

	}



	float angleTowards = MathHelper::moveTowards(0.0f, angleRad, maxRadiansDelta);



	float lengthTowards = MathHelper::moveTowards(curLength, targetLength, maxMagnitudeDelta);



	return Quaternion::createFromAxisAngle(axis, MathHelper::toDegrees(angleTowards)) * current * lengthTowards;

}



Vector3 Vector3::slerp(const Vector3 & value1, const Vector3 & value2, float amount)

{



	float value1Length = value1.length();

	float value2Length = value2.length();



	if (value1Length * value2Length < MathHelper::kEpsilon)

	{

		return Vector3::lerp(value1, value2, amount);

	}



	Vector3 afterNormalizeValue1 = value1 / value1Length;

	Vector3 afterNormalizeValue2 = value2 / value2Length;



	float angle = Vector3::angle(afterNormalizeValue1, afterNormalizeValue2);



	Vector3 axis = Vector3::cross(afterNormalizeValue1, afterNormalizeValue2);



	if (axis.lengthSquared() < (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		axis = Vector3::cross(Vector3::right, afterNormalizeValue1);

	}



	return Quaternion::createFromAxisAngle(axis, angle * amount) * afterNormalizeValue1 * MathHelper::lerp(value1Length, value2Length, amount);

}



Vector3 Vector3::slerpUnclamped(const Vector3 & value1, const Vector3 & value2, float amount)

{

	float value1Length = value1.length();

	float value2Length = value2.length();



	if (value1Length * value2Length < MathHelper::kEpsilon)

	{

		return Vector3::lerp(value1, value2, amount);

	}



	Vector3 afterNormalizeValue1 = value1 / value1Length;

	Vector3 afterNormalizeValue2 = value2 / value2Length;



	float angle = Vector3::angle(afterNormalizeValue1, afterNormalizeValue2);



	Vector3 axis = Vector3::cross(afterNormalizeValue1, afterNormalizeValue2);



	if (axis.lengthSquared() < (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		axis = Vector3::cross(Vector3::right, afterNormalizeValue1);

	}



	return Quaternion::createFromAxisAngle(axis, angle * amount) * afterNormalizeValue1 * MathHelper::lerpUnclamped(value1Length, value2Length, amount);

}



Vector3 Vector3::smoothDamp(const Vector3 & cur, const Vector3 & target, Vector3 * curVelocity, float smoothTime, float deltaTime, float maxSpeed)

{

	float output_x = 0.0f;

	float output_y = 0.0f;

	float output_z = 0.0f;



	smoothTime = std::fmax(0.0001f, smoothTime);

	float omega = 2.0f / smoothTime;



	float x = omega * deltaTime;

	float exp = 1.0f / (1.0f + x + 0.48f * x + 0.235f * x * x * x);



	float change_x = cur.x - target.x;

	float change_y = cur.y - target.y;

	float change_z = cur.z - target.z;



	float maxChange = maxSpeed * smoothTime;



	float maxChangeSq = maxChange * maxChange;

	float sqrlen = (change_x * change_x) + (change_y * change_y) + (change_z * change_z);



	if (sqrlen > maxChangeSq)

	{

		float len = std::sqrt(sqrlen);

		change_x = change_x / len * maxChange;

		change_y = change_y / len * maxChange;

		change_z = change_z / len * maxChange;

	}



	Vector3 result = target;



	result.x = cur.x - change_x;

	result.y = cur.y - change_y;

	result.z = cur.z - change_z;



	float temp_x = ((*curVelocity).x + omega * change_x) * deltaTime;

	float temp_y = ((*curVelocity).y + omega * change_y) * deltaTime;

	float temp_z = ((*curVelocity).z + omega * change_z) * deltaTime;



	(*curVelocity).x = ((*curVelocity).x - omega * temp_x) * exp;

	(*curVelocity).y = ((*curVelocity).y - omega * temp_y) * exp;

	(*curVelocity).z = ((*curVelocity).z - omega * temp_z) * exp;



	output_x = result.x + (change_x + temp_x) * exp;

	output_y = result.y + (change_y + temp_y) * exp;

	output_z = result.z + (change_z + temp_z) * exp;



	float origMinusCurrent_x = target.x - cur.x;

	float origMinusCurrent_y = target.y - cur.y;

	float origMinusCurrent_z = target.z - cur.z;



	float outMinusOrig_x = output_x - target.x;

	float outMinusOrig_y = output_y - target.y;

	float outMinusOrig_z = output_z - target.z;



	if ((origMinusCurrent_x * outMinusOrig_x)

		+ (origMinusCurrent_y * outMinusOrig_y)

		+ (origMinusCurrent_z * outMinusOrig_z) > 0)

	{

		output_x = target.x;

		output_y = target.y;

		output_z = target.z;



		(*curVelocity).x = (output_x - target.x) / deltaTime;

		(*curVelocity).y = (output_y - target.y) / deltaTime;

		(*curVelocity).z = (output_z - target.z) / deltaTime;

	}

	return Vector3(output_x, output_y, output_z);

}



Vector3 Vector3::transform(const Vector3 & position, const Matrix4 & matrix)

{

	float w = position.x * matrix.m[0][3] + position.y * matrix.m[1][3] + position.z * matrix.m[2][3] + matrix.m[3][3];

	return Vector3(

		(position.x * matrix.m[0][0] + position.y * matrix.m[1][0] + position.z * matrix.m[2][0] + matrix.m[3][0]) / w,

		(position.x * matrix.m[0][1] + position.y * matrix.m[1][1] + position.z * matrix.m[2][1] + matrix.m[3][1]) / w,

		(position.x * matrix.m[0][2] + position.y * matrix.m[1][2] + position.z * matrix.m[2][2] + matrix.m[3][2]) / w);

}



Vector3 Vector3::transformNormal(const Vector3 & position, const Matrix4 & matrix)

{

	return Vector3(

		position.x * matrix.m[0][0] + position.y * matrix.m[1][0] + position.z * matrix.m[2][0],

		position.x * matrix.m[0][1] + position.y * matrix.m[1][1] + position.z * matrix.m[2][1],

		position.x * matrix.m[0][2] + position.y * matrix.m[1][2] + position.z * matrix.m[2][2]);

}



Vector3 Vector3::createFromYawPitch(float yaw, float pitch)

{

	return Vector3(

		MathHelper::cos(pitch) * MathHelper::sin(yaw),

		-MathHelper::sin(pitch),

		MathHelper::cos(pitch) * MathHelper::cos(yaw));

}



float Vector3::angle(const Vector3& from, const Vector3& to)

{

	float dotValue = dot(normalize(from), normalize(to));

	// åvéZåÎç∑Ç≈ì‡êœÇÃåãâ Ç™-1.0Å`1.0Çí¥Ç¶ÇÈÇ∆acosÇÕnanÇï‘Ç∑

	return MathHelper::acos(MathHelper::clamp(dotValue, -1.0f, 1.0f));

}



float Vector3::length() const

{

	return std::sqrt(lengthSquared());

}



float Vector3::lengthSquared() const

{

	return dot(*this, *this);

}



Vector3 Vector3::normalized() const

{

	const float len = length();

	if (len < MathHelper::kEpsilon) return Vector3::zero;

	return Vector3(x / len, y / len, z / len);

}



bool Vector3::equals(const Vector3 & other)

{

	return(

		MathHelper::equals(x, other.x) &&

		MathHelper::equals(y, other.y) &&

		MathHelper::equals(z, other.z)

		);

}



void Vector3::setValue(float x, float y, float z)

{

	Vector3::x = x;

	Vector3::y = y;

	Vector3::z = z;

}



float Vector3::pitch() const

{

	float len = length();

	if (MathHelper::equals(len, 0.0f)) return 0.0f;

	return MathHelper::asin(-y / len);

}



float Vector3::yaw() const

{

	if (MathHelper::equals(length(), 0.0f)) return 0.0f;

	return MathHelper::atan(x, z);

}



std::string Vector3::toString() const

{

	return "[x:" + std::to_string(x) + ", y:" + std::to_string(y) + ", z:" + std::to_string(z) + "]";

}



Vector3 operator - (const Vector3& v)

{

	return Vector3(-v.x, -v.y, -v.z);

}



Vector3& operator += (Vector3& v1, const Vector3& v2)

{

	v1.x += v2.x;

	v1.y += v2.y;

	v1.z += v2.z;

	return v1;

}



Vector3& operator -= (Vector3& v1, const Vector3& v2)

{

	v1.x -= v2.x;

	v1.y -= v2.y;

	v1.z -= v2.z;

	return v1;

}



Vector3& operator *= (Vector3& v, float s)

{

	v.x *= s;

	v.y *= s;

	v.z *= s;

	return v;

}



Vector3& operator /= (Vector3& v, float s)

{

	v.x /= s;

	v.y /= s;

	v.z /= s;

	return v;

}



Vector3 operator + (Vector3 v1, const Vector3& v2)

{

	return v1 += v2;

}



Vector3 operator - (Vector3 v1, const Vector3& v2)

{

	return v1 -= v2;

}



Vector3 operator * (Vector3 v, float s)

{

	return v *= s;

}



Vector3 operator * (float s, Vector3 v)

{

	return v *= s;

}



Vector3 operator / (Vector3 v, float s)

{

	return v /= s;

}



bool operator==(const Vector3 & lhs, const Vector3 & rhs)

{

	return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z));

}



bool operator!=(const Vector3 & lhs, const Vector3 & rhs)

{

	return !(lhs == rhs);

}



std::ostream & operator<<(std::ostream & os, const Vector3 & vector3)

{

	return os << vector3.toString();

}