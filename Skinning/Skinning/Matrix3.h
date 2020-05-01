#pragma once
#ifndef MATRIX3_H_

#define MATRIX3_H_



#include <ostream>



struct Vector2;



struct Matrix3

{

public:



	Matrix3() = default;



	Matrix3(

		float m11, float m12, float m13,

		float m21, float m22, float m23,

		float m31, float m32, float m33);



	// ゼロ行列

	static const Matrix3 zero;

	// 単位行列

	static const Matrix3 identity;



	// 移動行列を生成

	static Matrix3 createTranslation(const Vector2& translation);

	// 回転行列を生成

	static Matrix3 createRotation(float degree);

	// スケール行列を生成

	static Matrix3 createScale(const Vector2& scale);

	// 移動、回転、スケールの値から行列を生成

	static Matrix3 createTRS(const Vector2& translation, float rotate, const Vector2& scale);



	// 逆行列を生成

	static Matrix3 calculateInvert(const Matrix3& matrix);

	//// 逆行列を生成（回転と平行移動のみ）

	//static Matrix3 calculateInvertFast(const Matrix3& matrix);

	// 行列の行と列を入れ替えます。

	static Matrix3 calculateTranspose(const Matrix3& matrix);



	// 2つの行列の対応する値の間を線形補間します

	//static Matrix3 lerp(const Matrix3& value1, const Matrix3& value2, float amount);



	// 平行移動ベクトルを設定

	void setTranslation(const Vector2& translation);

	// 移動、回転、スケールの値を再設定

	void setTRS(const Vector2& translation, float rotate, const Vector2& scale);



	// 平行移動ベクトルを計算

	Vector2 calculateTranslation() const;

	// 回転を計算

	float calculateRotation() const;

	// スケール値を計算

	Vector2 calculateScale() const;



	// 上方ベクトルを計算

	Vector2 calculateUp() const;

	// 右ベクトルを計算

	Vector2 calculateRight() const;



	// フォーマットされた文字列に変換

	std::string toString() const;



public:



	float m[3][3]{};

};



Matrix3 operator - (const Matrix3& m);



Matrix3& operator += (Matrix3& m1, const Matrix3& m2);

Matrix3& operator -= (Matrix3& m1, const Matrix3& m2);

Matrix3& operator *= (Matrix3& m1, const Matrix3& m2);

Matrix3& operator *= (Matrix3& m, float s);

Matrix3& operator /= (Matrix3& m, float s);



Matrix3 operator + (Matrix3 m1, const Matrix3& m2);

Matrix3 operator - (Matrix3 m1, const Matrix3& m2);

Matrix3 operator * (Matrix3 m1, const Matrix3& m2);

Matrix3 operator * (Matrix3 m, float s);

Matrix3 operator * (float s, Matrix3 m);

Matrix3 operator / (Matrix3 m, float s);



std::ostream& operator<<(std::ostream& os, const Matrix3& matrix);

#endif