#include "Color.h"

#include <algorithm>

#include "MathHelper.h"



const Color Color::black(0.0f, 0.0f, 0.0f, 1.0f);



const Color Color::white(1.0f, 1.0f, 1.0f, 1.0f);



const Color Color::red(1.0f, 0.0f, 0.0f, 1.0f);



const Color Color::green(0.0f, 1.0f, 0.0f, 1.0f);



const Color Color::blue(0.0f, 0.0f, 1.0f, 1.0f);



Color::Color(float r, float g, float b, float a)

	: r(r)

	, g(g)

	, b(b)

	, a(a)

{

}



Color Color::lerp(const Color& value1, const Color& value2, float amount)

{

	return Color(

		MathHelper::lerp(value1.r, value2.r, amount),

		MathHelper::lerp(value1.g, value2.g, amount),

		MathHelper::lerp(value1.b, value2.b, amount),

		MathHelper::lerp(value1.a, value2.a, amount));

}



Color Color::clamp(const Color & value1, const Color & min, const Color & max)

{

	return Color(

		MathHelper::clamp(value1.r, min.r, max.r),

		MathHelper::clamp(value1.g, min.g, max.g),

		MathHelper::clamp(value1.b, min.b, max.b),

		MathHelper::clamp(value1.a, min.a, max.a));

}



Color Color::calculateMin(const Color & value1, const Color & value2)

{

	return Color(

		std::min(value1.r, value2.r),

		std::min(value1.g, value2.g),

		std::min(value1.b, value2.b),

		std::min(value1.a, value2.a));

}



Color Color::calculateMax(const Color & value1, const Color & value2)

{

	return Color(

		std::max(value1.r, value2.r),

		std::max(value1.g, value2.g),

		std::max(value1.b, value2.b),

		std::max(value1.a, value2.a));

}



std::string Color::toString() const

{

	return  "[red:" + std::to_string(r)

		+ ", green:" + std::to_string(g)

		+ ", blue:" + std::to_string(b)

		+ ", alpha:" + std::to_string(a)

		+ "]";

}



Color& operator += (Color & c1, const Color & c2)

{

	c1.r += c2.r;

	c1.g += c2.g;

	c1.b += c2.b;

	c1.a += c2.a;

	return c1;

}



Color& operator -= (Color & c1, const Color & c2)

{

	c1.r -= c2.r;

	c1.g -= c2.g;

	c1.b -= c2.b;

	c1.a -= c2.a;

	return c1;

}



Color& operator *= (Color & c1, const Color & c2)

{

	c1.r *= c2.r;

	c1.g *= c2.g;

	c1.b *= c2.b;

	c1.a *= c2.a;

	return c1;

}



Color& operator *= (Color & c, float s)

{

	c.r *= s;

	c.g *= s;

	c.b *= s;

	c.a *= s;

	return c;

}



Color& operator /= (Color & c, float s)

{

	c.r /= s;

	c.g /= s;

	c.b /= s;

	c.a /= s;

	return c;

}



Color operator + (Color c1, const Color & c2)

{

	return c1 += c2;

}



Color operator - (Color c1, const Color & c2)

{

	return c1 -= c2;

}



Color operator * (Color c1, const Color & c2)

{

	return c1 *= c2;

}



Color operator * (Color c, float s)

{

	return c *= s;

}



Color operator * (float s, Color c)

{

	return c *= s;

}



Color operator / (Color c, float s)

{

	return c /= s;

}



std::ostream & operator<<(std::ostream & os, const Color & color)

{

	return os << color.toString();

}