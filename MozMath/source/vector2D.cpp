//==============================================================================
//
// 2D�x�N�g��[vector2D.cpp]
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
		// �R���X���N�^
		//------------------------------------------------------------------------------
		Vector2D::Vector2D(float _x, float _y)
		{
			v.x = _x;
			v.y = _y;
		}

		//------------------------------------------------------------------------------
		// �x�N�^�X�J���{
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator*(float f) const
		{
			return Vector2D(v.x*f, v.y*f);
		}

		//------------------------------------------------------------------------------
		// �x�N�^�X�J���{
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator/(float f) const
		{
			// �t���Ƃ��Čv�Z�����ق�������
			float fInv = 1.f / f;
			return Vector2D(v.x*fInv, v.y*fInv);
		}

		//------------------------------------------------------------------------------
		// �x�N�^���m�̑����Z
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator+(const Vector2D& _v) const
		{
			return Vector2D(v.x + _v.v.x, v.y + _v.v.y);
		}

		//------------------------------------------------------------------------------
		// �x�N�^���m�̍�
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator-(const Vector2D& _v) const
		{
			return Vector2D(v.x - _v.v.x, v.y - _v.v.y);
		}

		//------------------------------------------------------------------------------
		// �x�N�^�̑O�ɏ�����Ă�+�L��
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator+() const
		{
			return *this;
		}

		//------------------------------------------------------------------------------
		// �x�N�^�̑O�ɏ�����Ă�-�L��
		//------------------------------------------------------------------------------
		Vector2D Vector2D::operator-() const
		{
			return Vector2D(-v.x, -v.y);
		}

		//------------------------------------------------------------------------------
		// �x�N�^�X�J���{�̑��
		//------------------------------------------------------------------------------
		Vector2D& Vector2D::operator*=(float f)
		{
			v.x *= f;
			v.y *= f;
			return *this;
		}

		//------------------------------------------------------------------------------
		// �x�N�^�X�J���{�̑��
		//------------------------------------------------------------------------------
		Vector2D& Vector2D::operator/=(float f)
		{
			float fInv = 1.f / f;
			v.x *= fInv;
			v.y *= fInv;
			return *this;
		}

		//------------------------------------------------------------------------------
		// �x�N�^�̘a�Ƒ���̍���
		//------------------------------------------------------------------------------
		Vector2D& Vector2D::operator+=(const Vector2D& _v)
		{
			v.x += _v.v.x;
			v.y += _v.v.y;
			return *this;
		}

		//------------------------------------------------------------------------------
		// �x�N�^�̘a�Ƒ���̍���
		//------------------------------------------------------------------------------
		Vector2D& Vector2D::operator-=(const Vector2D& _v)
		{
			v.x -= _v.v.x;
			v.y -= _v.v.y;
			return *this;
		}

		//------------------------------------------------------------------------------
		// ����
		//------------------------------------------------------------------------------
		float Vector2D::Dot(const Vector2D& vec1, const Vector2D& vec2)
		{
			return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y;
		}

		//------------------------------------------------------------------------------
		// �O��
		//------------------------------------------------------------------------------
		float Vector2D::Cross(const Vector2D& vec1, const Vector2D& vec2)
		{
			return vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
		}

		//------------------------------------------------------------------------------
		// �����擾
		//------------------------------------------------------------------------------
		float Vector2D::GetLength(const Vector2D& vec)
		{
			return sqrtf(POW2(vec.v.x) + POW2(vec.v.y));
		}

		//------------------------------------------------------------------------------
		// ���K��
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
		// ����
		//------------------------------------------------------------------------------
		float Vector2D::Dot(const Vector2D& vec)
		{
			return Dot(*this, vec);
		}

		//------------------------------------------------------------------------------
		// �O��	
		//------------------------------------------------------------------------------
		float Vector2D::Cross(const Vector2D& vec)
		{
			return Cross(*this, vec);
		}

		//------------------------------------------------------------------------------
		// ���K��
		//------------------------------------------------------------------------------
		void Vector2D::Normalize(void)
		{
			Normalize(this, *this);
		}

		//------------------------------------------------------------------------------
		// �����擾
		//------------------------------------------------------------------------------
		float Vector2D::GetLength(void)
		{
			return GetLength(*this);
		}
	}
}