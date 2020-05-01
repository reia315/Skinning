#include "Quaternion.h"

#include <cmath>

#include <stdexcept>



#include "MathHelper.h"

#include "Matrix4.h"



const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

inline Vector3 internalMakePositive(Vector3 euler);



Quaternion::Quaternion(float x, float y, float z, float w)

	: x(x), y(y), z(z), w(w)

{

}



Quaternion Quaternion::createFromEulerAngle(const Vector3& euler)

{

	Vector3 v = Vector3(

		MathHelper::toRadians(euler.x),

		MathHelper::toRadians(euler.y),

		MathHelper::toRadians(euler.z)

	);



	float c1 = std::cos(v.x * 0.5f);

	float c2 = std::cos(v.y * 0.5f);

	float c3 = std::cos(v.z * 0.5f);

	float s1 = std::sin(v.x * 0.5f);

	float s2 = std::sin(v.y * 0.5f);

	float s3 = std::sin(v.z * 0.5f);



	float result_x = s1 * c2 * c3 + c1 * s2 * s3;

	float result_y = c1 * s2 * c3 - s1 * c2 * s3;

	float result_z = c1 * c2 * s3 - s1 * s2 * c3;

	float result_w = c1 * c2 * c3 + s1 * s2 * s3;



	return Quaternion(result_x, result_y, result_z, result_w);

}



Quaternion Quaternion::createFromAxisAngle(const Vector3 & axis, float degree)

{

	float sin = MathHelper::sin(degree / 2.0f);



	return Quaternion(

		sin * axis.x,

		sin * axis.y,

		sin * axis.z,

		MathHelper::cos(degree / 2.0f)

	);

}



Quaternion Quaternion::createFromRotationMatrix(const Matrix4 & matrix)

{

	Quaternion result;



	float tr = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2] + 1.0f;



	if (tr >= 1.0f)

	{

		float fourD = 2.0f * std::sqrt(tr);



		result.x = (matrix.m[1][2] - matrix.m[2][1]) / fourD;

		result.y = (matrix.m[2][0] - matrix.m[0][2]) / fourD;

		result.z = (matrix.m[0][1] - matrix.m[1][0]) / fourD;

		result.w = fourD / 4.0f;



		return result;

	}



	int i = 0;



	if (matrix.m[0][0] <= matrix.m[1][1])

	{

		i = 1;

	}



	if (matrix.m[2][2] > matrix.m[i][i])

	{

		i = 2;

	}



	int j = (i + 1) % 3;

	int k = (j + 1) % 3;



	tr = matrix.m[i][i] - matrix.m[j][j] - matrix.m[k][k] + 1.0f;



	float fourD = 2.0f * std::sqrt(tr);



	result[i] = fourD / 4.0f;

	result[j] = (matrix.m[j][i] + matrix.m[i][j]) / fourD;

	result[k] = (matrix.m[k][i] + matrix.m[i][k]) / fourD;

	result[3] = (matrix.m[j][k] - matrix.m[k][j]) / fourD;



	return result;

}



Quaternion Quaternion::createFromToRotation(const Vector3 & fromDirection, const Vector3 & toDirection)

{

	float angle = Vector3::angle(fromDirection, toDirection);



	if (angle < MathHelper::kEpsilon)

	{

		return identity;

	}



	Vector3 axis = Vector3::cross(fromDirection, toDirection);



	if (axis.lengthSquared() < (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		axis = Vector3::cross(Vector3::forwardLH, fromDirection);



		if (axis.lengthSquared() < (MathHelper::kEpsilon * MathHelper::kEpsilon))

		{

			axis = Vector3::cross(Vector3::right, fromDirection);

		}

	}

	return createFromAxisAngle(axis, angle);

}



Quaternion Quaternion::createLookRotation(const Vector3 & view, const Vector3 & up)

{

	Vector3 forward = Vector3::normalize(view);



	if (forward.lengthSquared() < (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		forward = Vector3::forwardLH;

	}



	Vector3 right = Vector3::cross(up, forward);



	if (right.lengthSquared() < (MathHelper::kEpsilon * MathHelper::kEpsilon))

	{

		right = Vector3::right;

	}



	Vector3 upward = Vector3::cross(forward, right);

	upward = Vector3::normalize(upward);



	right = Vector3::cross(upward, forward);



	Quaternion result;



	float radicand = right.x + upward.y + forward.z;



	if (radicand > 0.0f)

	{

		result.w = std::sqrt(1.0f + radicand) * 0.5f;

		float recip = 1.0f / (4.0f * result.w);

		result.x = (upward.z - forward.y) * recip;

		result.z = (forward.x - right.z) * recip;

		result.y = (right.y - upward.x) * recip;

	}

	else if (right.x >= upward.y && right.x >= forward.z)

	{

		result.x = std::sqrt(1.0f + right.x + upward.y - forward.z) * 0.5f;

		float recip = 1.0f / (4.0f * result.x);

		result.w = (upward.z - forward.y) * recip;

		result.z = (forward.x + right.z) * recip;

		result.y = (right.y + upward.z) * recip;

	}

	else if (upward.y > forward.z)

	{

		result.y = std::sqrt(1.0f - right.x + upward.y - forward.z) * 0.5f;

		float recip = 1.0f / (4.0f * result.y);

		result.z = (upward.z + forward.y) * recip;

		result.w = (forward.x - right.z) * recip;

		result.x = (right.y + upward.x) * recip;

	}

	else

	{

		result.z = std::sqrt(1.0f - right.x - upward.y + forward.z) * 0.5f;

		float recip = 1.0f / (4.0f * result.z);

		result.y = (upward.z + forward.y) * recip;

		result.x = (forward.x + right.z) * recip;

		result.w = (right.y - upward.x) * recip;

	}

	return result;

}



Quaternion Quaternion::createRotateTowards(const Quaternion & from, const Quaternion & to, float maxDegreesDelta)

{

	Vector3 eulerFrom = from.calculateEulerAngle();



	Vector3 eulerTo = to.calculateEulerAngle();



	Vector3 rotatedEuler = Vector3::rotateTowards(eulerFrom, eulerTo, maxDegreesDelta, 0.0f);



	return Quaternion::createFromEulerAngle(rotatedEuler);

}



Quaternion Quaternion::calculateInvert(const Quaternion & rotation)

{

	return Quaternion(-rotation.x, -rotation.y, -rotation.z, rotation.w);

}



float Quaternion::calculateAngle(const Quaternion & q1, const Quaternion & q2)

{

	float afterDot = calculateDot(q1, q2);



	if (afterDot > 1.0f - MathHelper::kEpsilon) return 0.0f;



	return MathHelper::acos(std::fmin(std::abs(afterDot), 1.0f)) * 2.0f;

}



float Quaternion::calculateDot(const Quaternion & q1, const Quaternion & q2)

{

	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

}



Quaternion Quaternion::normalize(const Quaternion & q)

{

	float len = q.calculateLength();



	if (len > MathHelper::kEpsilon)

	{

		return Quaternion(q) / len;

	}

	return Quaternion(q);

}



Quaternion Quaternion::lerp(const Quaternion & value1, const Quaternion & value2, float t)

{

	Vector3 eulerValue1 = value1.calculateEulerAngle();

	Vector3 eulerValue2 = value2.calculateEulerAngle();



	Vector3 afterLerp = Vector3::lerp(eulerValue1, eulerValue2, t);



	return Quaternion::createFromEulerAngle(afterLerp);

}



Quaternion Quaternion::lerpUnclamped(const Quaternion & value1, const Quaternion & value2, float t)

{

	Vector3 eulerValue1 = value1.calculateEulerAngle();

	Vector3 eulerValue2 = value2.calculateEulerAngle();



	Vector3 afterLerp = Vector3::lerpUnclamped(eulerValue1, eulerValue2, t);



	return Quaternion::createFromEulerAngle(afterLerp);

}



Quaternion Quaternion::slerp(const Quaternion & value1, const Quaternion & value2, float amount)

{

	return slerpUnclamped(value1, value2, MathHelper::clamp(amount, 0.0f, 1.0f));

}



Quaternion Quaternion::slerpUnclamped(const Quaternion & value1, const Quaternion & value2, float amount)

{

	float cos = calculateDot(value1, value2);



	float sign = 1.0f;



	if (cos < 0.0f)

	{

		cos = -cos;

		sign = -1.0f;

	}



	float t0 = 1.0f - amount;

	float t1 = amount;



	if (cos < 0.999f)

	{

		float theta = std::acos(cos);

		t0 = std::sin(theta * t0) / std::sin(theta);

		t1 = std::sin(theta * t1) / std::sin(theta);

	}

	return Quaternion::normalize(((value1 * t0) + (value2 * sign * t1)));



	/*Quaternion t2 = value2;

	if (cos < 0.0f)

	{

		cos = -cos;

		t2 = -value2;

	}



	float k0 = 1.0f - amount;

	float k1 = amount;

	if ((1.0f - cos) > 0.001f)

	{

		float theta = std::acos(cos);

		k0 = std::sin(theta*k0) / std::sin(theta);

		k1 = std::sin(theta*k1) / std::sin(theta);

	}

	return (value1 * k0) + (t2 * k1);*/

}



float Quaternion::calculateLength() const

{

	return std::sqrt(calculateDot(*this, *this));

}



// ※未デバッグ

Vector3 Quaternion::calculateEulerAngle() const

{

	Vector3 ax = *this * Vector3{ 1.0f, 0.0f, 0.0f };

	Vector3 ay = *this * Vector3{ 0.0f, 1.0f, 0.0f };

	Vector3 az = *this * Vector3{ 0.0f, 0.0f, 1.0f };

	Vector3 result = Vector3(0.0f, 0.0f, 0.0f);

	if (az.y < 1.0f) {

		if (az.y > -1.0f) {

			result.x = std::asin(-az.y);

			result.y = std::atan2(az.x, az.z);

			result.z = std::atan2(ax.y, ay.y);

		}

		else {

			// Not a unique solution.

			result.x = MathHelper::pi / 2.0f;

			result.y = -std::atan2(-ay.x, ax.x);

			result.z = 0.0f;

		}

	}

	else {

		// Not a unique solution.

		result.x = -MathHelper::pi / 2.0f;

		result.y = std::atan2(-ay.x, ax.x);

		result.z = 0.0f;

	}

	return internalMakePositive(Vector3(

		MathHelper::toDegrees(result.x),

		MathHelper::toDegrees(result.y),

		MathHelper::toDegrees(result.z)

	));



	//float x2 = x * x;

	//float y2 = y * y;

	//float z2 = z * z;

	//

	//float xy = x * y;

	//float xz = x * z;

	//float yz = y * z;

	//float wx = w * x;

	//float wy = w * y;

	//float wz = w * z;

	//

	//// 1 - 2y^2 - 2z^2

	//float m00 = 1.0f - (2.0f * y2) - (2.0f * z2);

	//

	//// 2xy + 2wz

	//float m01 = (2.0f * xy) + (2.0f * wz);

	//

	//// 2xy - 2wz

	//float m10 = (2.0f * xy) - (2.0f * wz);

	//

	//// 1 - 2x^2 - 2z^2

	//float m11 = 1.0f - (2.0f * x2) - (2.0f * z2);

	//

	//// 2xz + 2wy

	//float m20 = (2.0f * xz) + (2.0f * wy);

	//

	//// 2yz+2wx

	//float m21 = (2.0f * yz) - (2.0f * wx);

	//

	//// 1 - 2x^2 - 2y^2

	//float m22 = 1.0f - (2.0f * x2) - (2.0f * y2);

	//

	//float tx, ty, tz;

	//

	//if (MathHelper::distance(m21, 1.0f) < MathHelper::kEpsilon)

	//{

	//	tx = MathHelper::toDegrees(MathHelper::pi / 2.0f);

	//	ty = 0.0f;

	//	tz = MathHelper::atan(m10, m00);

	//}

	//else if (MathHelper::distance(m21, -1.0f) < MathHelper::kEpsilon)

	//{

	//	tx = MathHelper::toDegrees(MathHelper::pi / 2.0f);

	//	ty = 0.0f;

	//	tz = MathHelper::atan(m10, m00);

	//}

	//else

	//{

	//	tx = MathHelper::asin(-m21);

	//	ty = MathHelper::atan(m20, m22);

	//	tz = MathHelper::atan(m01, m11);

	//}

	//

	//return Vector3(tx, ty, tz);

}



void Quaternion::setValue(float x, float y, float z, float w)

{

	this->x = x;

	this->y = y;

	this->z = z;

	this->w = w;

}



void Quaternion::setFromToRotation(const Vector3 & fromDirection, const Vector3 & toDirection)

{

	*this = createFromToRotation(fromDirection, toDirection);

}

void Quaternion::setLookRotation(const Vector3 & view, const Vector3 & up)

{

	*this = createLookRotation(view, up);

}



bool Quaternion::equals(const Quaternion & other) const

{

	return (

		std::abs(x - other.x) < MathHelper::kEpsilon &&

		std::abs(y - other.y) < MathHelper::kEpsilon &&

		std::abs(z - other.z) < MathHelper::kEpsilon &&

		std::abs(w - other.w) < MathHelper::kEpsilon

		);

}



std::string Quaternion::toString() const

{

	return "[x:" + std::to_string(x) + ", y:" + std::to_string(y) + ", z:" + std::to_string(z) + ", w:" + std::to_string(w) + "]";;

}



float Quaternion::operator[](int index) const

{

	switch (index)

	{

	case 0: return x; break;

	case 1: return y; break;

	case 2: return z; break;

	case 3: return w; break;

	}

	throw std::runtime_error("quaternion operator[] out of range");

}



float & Quaternion::operator[](int index)

{

	switch (index)

	{

	case 0: return x; break;

	case 1: return y; break;

	case 2: return z; break;

	case 3: return w; break;

	}

	throw std::runtime_error("quaternion operator[] out of range");

}



Quaternion operator - (const Quaternion& q)

{

	return Quaternion(-q.x, -q.y, -q.z, -q.w);

}



Quaternion& operator += (Quaternion& q1, const Quaternion& q2)

{

	q1.x += q2.x;

	q1.y += q2.y;

	q1.z += q2.z;

	q1.w += q2.w;

	return q1;

}



Quaternion& operator -= (Quaternion& q1, const Quaternion& q2)

{

	q1.x -= q2.x;

	q1.y -= q2.y;

	q1.z -= q2.z;

	q1.w -= q2.w;

	return q1;

}

Quaternion& operator *= (Quaternion& q, float s)

{

	q.x *= s;

	q.y *= s;

	q.z *= s;

	q.w *= s;

	return q;

}



Quaternion& operator /= (Quaternion& q, float s)

{

	q.x /= s;

	q.y /= s;

	q.z /= s;

	q.w /= s;

	return q;

}



Quaternion& operator *= (Quaternion& q1, const Quaternion& q2)

{

	q1 = Quaternion(

		q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,

		-q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,

		q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,

		-q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w);

	return q1;

}



Quaternion operator + (Quaternion q1, const Quaternion& q2)

{

	return q1 += q2;

}



Quaternion operator - (Quaternion q1, const Quaternion& q2)

{

	return q1 -= q2;

}



Quaternion operator * (Quaternion q1, const Quaternion& q2)

{

	return q1 *= q2;

}



Quaternion operator * (Quaternion q, float s)

{

	return q *= s;

}



Quaternion operator * (float s, Quaternion q)

{

	return q *= s;

}



Quaternion operator / (Quaternion q, float s)

{

	return q /= s;

}



Vector3 operator*(const Quaternion & rotation, const Vector3 & point)

{

	float x = rotation.x * 2.0f;

	float y = rotation.y * 2.0f;

	float z = rotation.z * 2.0f;



	float xx = rotation.x * x;

	float yy = rotation.y * y;

	float zz = rotation.z * z;



	float xy = rotation.x * y;

	float xz = rotation.x * z;

	float yz = rotation.y * z;



	float wx = rotation.w * x;

	float wy = rotation.w * y;

	float wz = rotation.w * z;



	Vector3 result;



	result.x = (1.0f - (yy + zz)) * point.x + (xy - wz) * point.y + (xz + wy) * point.z;

	result.y = (xy + wz) * point.x + (1.0f - (xx - zz)) * point.y + (yz - wx) * point.z;

	result.z = (xz - wy) * point.x + (yz + wx) * point.y + (1.0f - (xx + yy)) * point.z;



	return result;

}



std::ostream & operator<<(std::ostream & os, const Quaternion & quaternion)

{

	return os << quaternion.toString();

}



// 0～360のプラスの角度に変換

inline Vector3 internalMakePositive(Vector3 euler)

{

	float negativeFlip = MathHelper::toDegrees(-0.0001f);

	float positiveFlip = 360.0f + negativeFlip;



	if (euler.x < negativeFlip)

		euler.x += 360.0f;

	else if (euler.x > positiveFlip)

		euler.x -= 360.0f;



	if (euler.y < negativeFlip)

		euler.y += 360.0f;

	else if (euler.y > positiveFlip)

		euler.y -= 360.0f;



	if (euler.z < negativeFlip)

		euler.z += 360.0f;

	else if (euler.z > positiveFlip)

		euler.z -= 360.0f;



	return euler;
}