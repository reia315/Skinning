//#pragma once
#ifndef MATH_HELPER_H_

#define MATH_HELPER_H_



class MathHelper

{

public:



	// πの値を表します

	static const float pi;



	// 凄く０に近い数字を表します

	static const float kEpsilon;



public:



	// ２つの値が大体同じだったら真を返す

	static bool equals(float value1, float value2);



	// 引数の値が正の数なら1、負の数なら-1を返します

	static float sign(float value);



	// 値を指定された範囲内に制限します

	static float clamp(float value, float min, float max);



	// 2つの値の差の絶対値を計算します

	static float distance(float value1, float value2);



	// 2つの値の間を線形補間します

	// ※amountの値が0～1の間にClampされる

	static float lerp(float value1, float value2, float amount);



	// 2つの値の間を線形補間します

	// ※amountの値がClampされない

	static float lerpUnclamped(float value1, float value2, float amount);



	// currentからtargetへ向けて移動します

	static float moveTowards(float current, float target, float maxDistanceDelta);



	// ラジアンを度に変換します。

	static float toDegrees(float radian);



	// 度をラジアンに変換します。

	static float toRadians(float degree);



	// 引数：度数法

	static float sin(float degree);



	// コサイン

	static float cos(float degree);



	// タンジェント

	static float tan(float degree);



	// 逆サイン（戻り値：度数法）

	static float asin(float s);



	// 逆コサイン（戻り値：度数法）

	static float acos(float c);



	// 逆タンジェント（戻り値：度数法）

	static float atan(float y, float x);

};

#endif