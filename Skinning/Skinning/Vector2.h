#ifndef VECTOR2_H_

#define VECTOR2_H_



#include <string>

#include <ostream>



struct Matrix3;



struct Vector2

{

	Vector2() = default;

	Vector2(float x, float y);

	explicit Vector2(float value);



	float operator[](int index) const;

	float& operator[](int index);



	// Vector2( 0,  0)

	static const Vector2 zero;

	// Vector2( 1,  1)

	static const Vector2 one;

	// Vector2( 0,  1)

	static const Vector2 up;

	// Vector2( 0, -1)

	static const Vector2 down;

	// Vector2(-1,  0)

	static const Vector2 left;

	// Vector2( 1,  0)

	static const Vector2 right;

	// Vector2(-INFINITY, -INFINITY)

	static const Vector2 negativeInfinity;

	// Vector2( INFINITY,  INFINITY)

	static const Vector2 positiveInfinity;



	// 指定ベクトルから単位ベクトルを作成します

	static Vector2 normalize(const Vector2& value);

	// 2つのベクトルのなす角を返す

	static float angle(const Vector2& vector1, const Vector2& vector2);

	// ２つのベクトルの間の角度を取得

	static float signedAngle(const Vector2& vector1, const Vector2& vector2);

	// 2つのベクトルの内積を計算します

	static float dot(const Vector2& vector1, const Vector2& vector2);

	// 2つのベクトルの外積を計算します

	static float cross(const Vector2& vector1, const Vector2& vector2);

	// 2つのベクトル間の距離を計算します

	static float distance(const Vector2& vector1, const Vector2& vector2);

	// 2 つの直交ベクトル間の距離を計算します。

	static float distanceSquared(const Vector2 & vector1, const Vector2 & vector2);

	// 2 つのベクトル間の線形補間を行います

	static Vector2 lerp(const Vector2& value1, const Vector2& value2, float amount);

	// 2 つのベクトル間の線形補間を行います

	static Vector2 lerpUnclamped(const Vector2& value1, const Vector2& value2, float amount);

	// currentからtargetへ向けて移動します

	static Vector2 moveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta);

	// currentからtargetへ向けて時間の経過で徐々にベクトルを変化させます

	static Vector2 smoothDamp(const Vector2& current, const Vector2& target, Vector2* currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

	// 一致する各成分ペアの最も低い値を含むベクトルを返します

	static Vector2 calculateMin(const Vector2& vector1, const Vector2& vector2);

	// 一致する各成分ペアの最も高い値を含むベクトルを返します

	static Vector2 calculateMax(const Vector2& vector1, const Vector2& vector2);

	//	値を指定された範囲内に制限します

	static Vector2 clamp(const Vector2& value1, const Vector2& min, const Vector2& max);

	// 大きさをmaxLengthまで制限した値を取得します

	static Vector2 clampLength(const Vector2& vector1, float maxLength);

	// 引数のベクトルの垂直方向のベクトルを取得

	static Vector2 perpendicular(const Vector2& inDirection);

	// 法線を基準にしてベクトルに反射したベクトルを取得します

	static Vector2 reflect(const Vector2& inDirection, const Vector2& inNormal);

	// 2つのベクトルの各成分を乗算する

	static Vector2 scale(const Vector2& vector1, const Vector2& vector2);

	// 引数の行列を使って移動させる

	static Vector2 transform(const Vector2& position, const Matrix3& matrix);



	// ベクトルの長さ

	float length() const;

	// ベクトルの長さの平方

	float lengthSquared() const;

	// 単位ベクトルを取得する

	Vector2 normalized() const;

	// 自身と引数の値が大体同じだったら真を返す

	bool equals(const Vector2 & other);

	// 値を再設定する

	void setValue(float x, float y);



	// ベクトルの値を文字列に起こした物を取得

	std::string toString() const;



public:



	float x{ 0.0f };

	float y{ 0.0f };

};



Vector2 operator - (const Vector2& v);



Vector2& operator += (Vector2& v1, const Vector2& v2);

Vector2& operator -= (Vector2& v1, const Vector2& v2);

Vector2& operator *= (Vector2& v, float s);

Vector2& operator *= (Vector2& v, const Matrix3& m);

Vector2& operator /= (Vector2& v, float s);



Vector2 operator + (Vector2 v1, const Vector2& v2);

Vector2 operator - (Vector2 v1, const Vector2& v2);

Vector2 operator * (Vector2 v, float s);

Vector2 operator * (float s, Vector2 v);

Vector2	operator * (const Vector2& v, const Matrix3& m);

Vector2 operator / (Vector2 v, float s);



bool operator == (const Vector2& lhs, const Vector2& rhs);

bool operator != (const Vector2& lhs, const Vector2& rhs);



std::ostream& operator<<(std::ostream& os, const Vector2& vector2);

#endif