#include "Matrix3.h"

#include <string>

#include "Vector2.h"

#include "MathHelper.h"



const Matrix3 Matrix3::identity

{

	1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f

};



const Matrix3 Matrix3::zero

{

	0.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f

};



Matrix3::Matrix3(

	float m11, float m12, float m13,

	float m21, float m22, float m23,

	float m31, float m32, float m33

)

{

	m[0][0] = m11; m[0][1] = m12; m[0][2] = m13;

	m[1][0] = m21; m[1][1] = m22; m[1][2] = m23;

	m[2][0] = m31; m[2][1] = m32; m[2][2] = m33;

}



Matrix3 Matrix3::createTranslation(const Vector2 & translation)

{

	return Matrix3(

		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,

		translation.x, translation.y, 1.0f

	);

}



Matrix3 Matrix3::createRotation(float degree)

{

	float sinValue = MathHelper::sin(degree);

	float cosValue = MathHelper::cos(degree);



	return Matrix3(

		cosValue, sinValue, 0.0f,

		-sinValue, cosValue, 0.0f,

		0.0f, 0.0f, 1.0f

	);

}



Matrix3 Matrix3::createScale(const Vector2 & scale)

{

	return Matrix3(

		scale.x, 0.0f, 0.0f,

		0.0f, scale.y, 0.0f,

		0.0f, 0.0f, 1.0f

	);

}



Matrix3 Matrix3::createTRS(const Vector2 & translation, float rotate, const Vector2 & scale)

{

	Matrix3 result = createRotation(rotate);



	result.m[0][0] *= scale.x;			result.m[0][1] *= scale.x;

	result.m[1][0] *= scale.y;			result.m[1][1] *= scale.y;

	result.m[2][0] = translation.x;		result.m[2][1] = translation.y;



	return result;

}



Matrix3 Matrix3::calculateInvert(const Matrix3 & matrix)

{

	float a1 = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2];

	float a2 = matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0];

	float a3 = matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1];

	float a4 = matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0];

	float a5 = matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2];

	float a6 = matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];



	float det = a1 + a2 + a3 - a4 - a5 - a6;



	if (det < MathHelper::kEpsilon)

	{

		return matrix; // ‹ts—ñ‚ª‘¶Ý‚µ‚È‚¢

	}

	float invDet = 1.0f / det;



	return Matrix3(

		(matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) * invDet,

		-(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]) * invDet,

		(matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]) * invDet,



		-(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]) * invDet,

		(matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]) * invDet,

		-(matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]) * invDet,



		(matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]) * invDet,

		-(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]) * invDet,

		(matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]) * invDet

	);

}



// TODO : ŽÀ‘•‚ðl‚¦‚é

//Matrix3 Matrix3::calculateInvertFast(const Matrix3 & matrix)

//{

//	Matrix3 invRotation = Matrix3::transpose(Matrix3(matrix).translation(Vector2::zero));

//

//	//Vector2 invTranslation = Vector2

//

//	return matrix;

//}



Matrix3 Matrix3::calculateTranspose(const Matrix3 & matrix)

{

	return Matrix3(

		matrix.m[0][0], matrix.m[1][0], matrix.m[2][0],

		matrix.m[0][1], matrix.m[1][1], matrix.m[2][1],

		matrix.m[0][2], matrix.m[1][2], matrix.m[2][2]

	);

}



void Matrix3::setTranslation(const Vector2 & translation)

{

	m[2][0] = translation.x; m[2][1] = translation.y;

}



void Matrix3::setTRS(const Vector2 & translation, float rotate, const Vector2 & scale)

{

	Matrix3 result = createTRS(translation, rotate, scale);



	for (int i = 0; i < 3; i++)

	{

		for (int j = 0; j < 3; j++)

		{

			m[i][j] = result.m[i][j];

		}

	}

}



Vector2 Matrix3::calculateTranslation() const

{

	return Vector2(m[2][0], m[2][1]);

}



float Matrix3::calculateRotation() const

{

	return MathHelper::acos(m[0][0] / calculateRight().length()) * MathHelper::sign(m[0][1]);

}



Vector2 Matrix3::calculateScale() const

{

	return Vector2(calculateRight().length(), calculateUp().length());

}



Vector2 Matrix3::calculateUp() const

{

	return Vector2(-m[1][0], -m[1][1]);

}



Vector2 Matrix3::calculateRight() const

{

	return Vector2(m[0][0], m[0][1]);

}



std::string Matrix3::toString() const

{

	return "[ m11:" + std::to_string(m[0][0]) + ", m12:" + std::to_string(m[0][1]) + ", m13:" + std::to_string(m[0][2]) + "\n"

		+ ", m21:" + std::to_string(m[1][0]) + ", m22:" + std::to_string(m[1][1]) + ", m23:" + std::to_string(m[1][2]) + "\n"

		+ ", m31:" + std::to_string(m[2][0]) + ", m32:" + std::to_string(m[2][1]) + ", m33:" + std::to_string(m[2][2]) + "]";

}



Matrix3 operator-(const Matrix3 & m)

{

	return Matrix3(

		-m.m[0][0], -m.m[0][1], -m.m[0][2],

		-m.m[1][0], -m.m[1][1], -m.m[1][2],

		-m.m[2][0], -m.m[2][1], -m.m[2][2]);

}



Matrix3 & operator+=(Matrix3 & m1, const Matrix3 & m2)

{

	for (int i = 0; i < 3; ++i)

	{

		for (int j = 0; j < 3; ++j)

		{

			m1.m[i][j] += m2.m[i][j];

		}

	}

	return m1;

}



Matrix3 & operator-=(Matrix3 & m1, const Matrix3 & m2)

{

	for (int i = 0; i < 3; ++i)

	{

		for (int j = 0; j < 3; ++j)

		{

			m1.m[i][j] -= m2.m[i][j];

		}

	}

	return m1;

}



Matrix3 & operator*=(Matrix3 & m1, const Matrix3 & m2)

{

	Matrix3 result = Matrix3::zero;

	for (int i = 0; i < 3; ++i)

	{

		for (int j = 0; j < 3; ++j)

		{

			for (int k = 0; k < 3; ++k)

			{

				result.m[i][j] += m1.m[i][k] * m2.m[k][j];

			}

		}

	}

	m1 = result;

	return m1;

}



Matrix3 & operator*=(Matrix3 & m, float s)

{

	for (int i = 0; i < 3; ++i)

	{

		for (int j = 0; j < 3; ++j)

		{

			m.m[i][j] *= s;

		}

	}

	return m;

}



Matrix3 & operator/=(Matrix3 & m, float s)

{

	return m *= (1.0f / s);

}



Matrix3 operator+(Matrix3 m1, const Matrix3 & m2)

{

	return m1 += m2;

}



Matrix3 operator-(Matrix3 m1, const Matrix3 & m2)

{

	return m1 -= m2;

}



Matrix3 operator*(Matrix3 m1, const Matrix3 & m2)

{

	return m1 *= m2;

}



Matrix3 operator*(Matrix3 m, float s)

{

	return m *= s;

}



Matrix3 operator*(float s, Matrix3 m)

{

	return m *= s;

}



Matrix3 operator/(Matrix3 m, float s)

{

	return m /= s;

}



std::ostream & operator<<(std::ostream & os, const Matrix3 & matrix)

{

	return os << matrix.toString();

}
