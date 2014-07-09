//==============================================================================
//
// カラー[color.cpp]
// Author : Yasuaki Yamashita : 2014/07/01
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "mozMath.h"

namespace moz
{
	namespace math
	{
		Color::Color(float r, float g, float b, float a)
		{
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
		}


		//------------------------------------------------------------------------------
		// カラースカラ倍
		//------------------------------------------------------------------------------
		Color Color::operator*(float f) const
		{
			return Color(c.r*f, c.g*f, c.b*f, c.a*f);
		}

		//------------------------------------------------------------------------------
		// カラースカラ倍
		//------------------------------------------------------------------------------
		Color Color::operator/(float f) const
		{
			// 逆数とって計算したほうが早い
			float fInv = 1.f / f;
			return Color(c.r*fInv, c.g*fInv, c.b*fInv, c.a*fInv);
		}

		//------------------------------------------------------------------------------
		// カラー同士の足し算
		//------------------------------------------------------------------------------
		Color Color::operator+(const Color& _v) const
		{
			return Color(c.r + _v.c.r, c.g + _v.c.g, c.b + _v.c.b, c.a + _v.c.a);
		}

		//------------------------------------------------------------------------------
		// カラー同士の差
		//------------------------------------------------------------------------------
		Color Color::operator-(const Color& _v) const
		{
			return Color(c.r - _v.c.r, c.g - _v.c.g, c.b - _v.c.b, c.a - _v.c.a);
		}

		//------------------------------------------------------------------------------
		// カラーの前に書かれてた+記号
		//------------------------------------------------------------------------------
		Color Color::operator+() const
		{
			return *this;
		}

		//------------------------------------------------------------------------------
		// カラーの前に書かれてた-記号
		//------------------------------------------------------------------------------
		Color Color::operator-() const
		{
			return Color(-c.r, -c.g, -c.b, -c.a);
		}

		//------------------------------------------------------------------------------
		// カラースカラ倍の代入
		//------------------------------------------------------------------------------
		Color& Color::operator*=(float f)
		{
			c.r *= f;
			c.g *= f;
			c.b *= f;
			c.a *= f;
			return *this;
		}

		//------------------------------------------------------------------------------
		// カラースカラ倍の代入
		//------------------------------------------------------------------------------
		Color& Color::operator/=(float f)
		{
			float fInv = 1.f / f;
			c.r *= fInv;
			c.g *= fInv;
			c.b *= fInv;
			c.a *= fInv;
			return *this;
		}

		//------------------------------------------------------------------------------
		// カラーの和と代入の合成
		//------------------------------------------------------------------------------
		Color& Color::operator+=(const Color& _v)
		{
			c.r += _v.c.r;
			c.g += _v.c.g;
			c.b += _v.c.b;
			c.a += _v.c.a;
			return *this;
		}

		//------------------------------------------------------------------------------
		// カラーの和と代入の合成
		//------------------------------------------------------------------------------
		Color& Color::operator-=(const Color& _v)
		{
			c.r -= _v.c.r;
			c.g -= _v.c.g;
			c.b -= _v.c.b;
			c.a -= _v.c.a;
			return *this;
		}

	}
}

//EOF