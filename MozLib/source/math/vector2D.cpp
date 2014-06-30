//==============================================================================
//
// 2Dベクトル[vector2D.cpp]
// Author : Yasuaki Yamashita : 2014/05/09
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include <math.h>
#include "mozMath.h"

namespace moz
{
	namespace math
	{
		//------------------------------------------------------------------------------
		// コンスラクタ
		//------------------------------------------------------------------------------
		Vector2D::Vector2D(float _x, float _y)
		{
			v.x = _x;
			v.y = _y;
		}

		//------------------------------------------------------------------------------
		// ベクタスカラ倍
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator*(float f) const
		{
			return Vector2D(v.x*f, v.y*f);
		}

		//------------------------------------------------------------------------------
		// ベクタスカラ倍
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator/(float f) const
		{
			// 逆数とって計算したほうが早い
			float fInv = 1.f / f;
			return Vector2D(v.x*fInv, v.y*fInv);
		}

		//------------------------------------------------------------------------------
		// ベクタ同士の足し算
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator+(const Vector2D& _v) const
		{
			return Vector2D(v.x + _v.v.x, v.y + _v.v.y);
		}

		//------------------------------------------------------------------------------
		// ベクタ同士の差
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator-(const Vector2D& _v) const
		{
			return Vector2D(v.x - _v.v.x, v.y - _v.v.y);
		}

		//------------------------------------------------------------------------------
		// ベクタの前に書かれてた+記号
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator+() const
		{
			return *this;
		}

		//------------------------------------------------------------------------------
		// ベクタの前に書かれてた-記号
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator-() const
		{
			return Vector2D(-v.x, -v.y);
		}

		//------------------------------------------------------------------------------
		// ベクタスカラ倍の代入
		//------------------------------------------------------------------------------
		Vector2D& Vector2D::operator*=(float f)
		{
			v.x *= f;
			v.y *= f;
			return *this;
		}

		//------------------------------------------------------------------------------
		// ベクタスカラ倍の代入
		//------------------------------------------------------------------------------
		Vector2D& Vector2D::operator/=(float f)
		{
			float fInv = 1.f / f;
			v.x *= fInv;
			v.y *= fInv;
			return *this;
		}

		//------------------------------------------------------------------------------
		// ベクタの和と代入の合成
		//------------------------------------------------------------------------------
		Vector2D& Vector2D::operator+=(const Vector2D& _v)
		{
			v.x += _v.v.x;
			v.y += _v.v.y;
			return *this;
		}

		//------------------------------------------------------------------------------
		// ベクタの和と代入の合成
		//------------------------------------------------------------------------------
		Vector2D& Vector2D::operator-=(const Vector2D& _v)
		{
			v.x -= _v.v.x;
			v.y -= _v.v.y;
			return *this;
		}

		//------------------------------------------------------------------------------
		// 内積
		//------------------------------------------------------------------------------
		float Vector2D::Dot(const Vector2D& vec1, const Vector2D& vec2)
		{
			return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y;
		}

		//------------------------------------------------------------------------------
		// 外積
		//------------------------------------------------------------------------------
		float Vector2D::Cross(const Vector2D& vec1, const Vector2D& vec2)
		{
			return vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
		}

		//------------------------------------------------------------------------------
		// 長さ取得
		//------------------------------------------------------------------------------
		float Vector2D::GetLength(const Vector2D& vec)
		{
			#define POW2(x) ((x)*(x))
			return sqrtf(POW2(vec.v.x) + POW2(vec.v.y));
		}

		//------------------------------------------------------------------------------
		// 正規化
		//------------------------------------------------------------------------------
		void Vector2D::Normalize(Vector2D* out, const Vector2D& in)
		{
			float len = GetLength(in);
			if (len != 0)
			{
				float scale = 1.f / len;
				out->v.x *= scale;
				out->v.y *= scale;
			}
		}

		//------------------------------------------------------------------------------
		// 内積
		//------------------------------------------------------------------------------
		float Vector2D::Dot(const Vector2D& vec)
		{
			return Dot(*this, vec);
		}

		//------------------------------------------------------------------------------
		// 外積	
		//------------------------------------------------------------------------------
		float Vector2D::Cross(const Vector2D& vec)
		{
			return Cross(*this, vec);
		}

		//------------------------------------------------------------------------------
		// 正規化
		//------------------------------------------------------------------------------
		void Vector2D::Normalize(void)
		{
			Normalize(this, *this);
		}

		//------------------------------------------------------------------------------
		// 長さ取得
		//------------------------------------------------------------------------------
		float Vector2D::GetLength(void)
		{
			return GetLength(*this);
		}
	}
}