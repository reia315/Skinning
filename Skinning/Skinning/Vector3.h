//#pragma once
#ifndef VECTOR3_H_

#define VECTOR3_H_



#include <string>

#include <ostream>



struct Matrix4;



struct Vector3

{

public:



	Vector3() = default;

	Vector3(float x, float y, float z);

	explicit Vector3(float value);



	float operator[](int index) const;

	float& operator[](int index);



	// Vector3( 0,  0,  0) 

	static const Vector3 zero;

	// Vector3( 1,  1,  1)

	static const Vector3 one;

	// Vector3( 0,  1,  0)

	static const Vector3 up;

	// Vector3( 0, -1,  0)

	static const Vector3 down;

	// Vector3(-1,  0,  0)

	static const Vector3 left;

	// Vector3( 1,  0,  0)

	static const Vector3 right;

	// Vector3( 0, 0,  1)（DirectX等）

	static const Vector3 forwardLH;

	// Vector3( 0, 0, -1)（DirectX等）

	static const Vector3 backwardLH;

	// Vector3( 0, 0, -1)（OpenGL等）

	static const Vector3 forwardRH;

	// Vector3( 0, 0,  1)（OpenGL等）

	static const Vector3 backwardRH;

	// Vector3(-INFINITY, -INFINITY, -INFINITY)

	static const Vector3 negativeInfinity;

	// Vector3( INFINITY,  INFINITY,  INFINITY)

	static const Vector3 positiveInfinity;



	// 指定ベクトルから単位ベクトルを作成します

	static Vector3 normalize(const Vector3& value);

	// ベクトルが正規化され他のベクトルと直交するようにします

	static void orthoNormalize(Vector3* normal, Vector3* tangent, Vector3* binormal);

	// ベクトルを別のベクトルに投影します。

	static Vector3 project(const Vector3& value, const Vector3& onNormal);

	// 平面に垂直な法線ベクトルによって定義される平面上にベクトルを射影します。

	static Vector3 projectOnPlane(const Vector3& value, const Vector3& planeNormal);

	// 2つのベクトルの内積を計算します

	static float dot(const Vector3& vector1, const Vector3& vector2);

	// 2つのベクトルの外積を計算します

	static Vector3 cross(const Vector3& vector1, const Vector3& vector2);

	// 2つのベクトル間の距離を計算します

	static float distance(const Vector3& vector1, const Vector3& vector2);

	// 2 つの直交ベクトル間の距離を計算します。 

	static float distanceSquared(const Vector3& vector1, const Vector3& vector2);

	// 2 つのベクトル間の線形補間を行います

	static Vector3 lerp(const Vector3& value1, const Vector3& value2, float amount);

	// 2 つのベクトル間の線形補間を行います

	static Vector3 lerpUnclamped(const Vector3& value1, const Vector3& value2, float amount);

	// currentからtargetへ向けて移動します

	static Vector3 moveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta);

	// 一致する各成分ペアの最も低い値を含むベクトルを返します

	static Vector3 calculateMin(const Vector3& vector1, const Vector3& vector2);

	// 一致する各成分ペアの最も高い値を含むベクトルを返します

	static Vector3 calculateMax(const Vector3& vector1, const Vector3& vector2);

	//	値を指定された範囲内に制限します

	static Vector3 clamp(const Vector3& value1, const Vector3& min, const Vector3& max);

	// 大きさをmaxLengthまで制限した値を取得します

	static Vector3 clampLength(const Vector3& value, float maxLength);

	// 法線を基準にしてベクトルに反射したベクトルを取得します

	static Vector3 reflect(const Vector3& inDirection, const Vector3& inNormal);

	// 2つのベクトルの各成分を乗算する

	static Vector3 scale(const Vector3& vector1, const Vector3& vector2);

	// Returns the signed angle in degrees between from and to

	static float signedAngle(const Vector3& from, const Vector3& to, const Vector3& axis);

	// 現在の位置 current から target に向けてベクトルを回転します。

	static Vector3 rotateTowards(const Vector3& current, const Vector3& target, float maxRadiansDelta, float maxMagnitudeDelta);

	// 球状に 2 つのベクトル間を補間します

	static Vector3 slerp(const Vector3& value1, const Vector3& value2, float amount);

	// 球状に 2 つのベクトル間を補間します

	static Vector3 slerpUnclamped(const Vector3& value1, const Vector3& value2, float amount);

	// 目的地に向かって時間の経過とともに徐々にベクトルを変化させます

	static Vector3 smoothDamp(const Vector3& cur, const Vector3& target, Vector3* curVelocity, float smoothTime, float deltaTime, float maxSpeed = INFINITY);

	// 指定された Matrix によって、Vector3をトランスフォームします

	static Vector3 transform(const Vector3& position, const Matrix4& matrix);

	// 指定された Matrix によって、法線ベクトルをトランスフォームします

	static Vector3 transformNormal(const Vector3& position, const Matrix4& matrix);

	// ヨー・ピッチからベクトルを作成

	static Vector3 createFromYawPitch(float yaw, float pitch);

	//  2 つのベクトル間の鋭角を返します

	static float angle(const Vector3& from, const Vector3& to);



	// ベクトルの長さを計算します

	float length() const;

	// ベクトルの長さの平方を計算します

	float lengthSquared() const;

	// 単位ベクトルを作成します

	Vector3 normalized() const;

	// 自身と引数の値が大体同じだったら真を返す

	bool equals(const Vector3& other);

	// 値を再設定する

	void setValue(float x, float y, float z);

	// ピッチを取得

	float pitch() const;

	// ヨーを取得

	float yaw() const;



	// ベクトルの値を文字列に起こした物を取得

	std::string toString() const;



public:



	float x{ 0.0f };

	float y{ 0.0f };

	float z{ 0.0f };

};



Vector3 operator - (const Vector3& v);



Vector3& operator += (Vector3& v1, const Vector3& v2);

Vector3& operator -= (Vector3& v1, const Vector3& v2);

Vector3& operator *= (Vector3& v, float s);

Vector3& operator /= (Vector3& v, float s);



Vector3 operator + (Vector3 v1, const Vector3& v2);

Vector3 operator - (Vector3 v1, const Vector3& v2);

Vector3 operator * (Vector3 v, float s);

Vector3 operator * (float s, Vector3 v);

Vector3 operator / (Vector3 v, float s);



bool operator == (const Vector3& lhs, const Vector3& rhs);

bool operator != (const Vector3& lhs, const Vector3& rhs);



std::ostream& operator<<(std::ostream& os, const Vector3& vector3);

#endif