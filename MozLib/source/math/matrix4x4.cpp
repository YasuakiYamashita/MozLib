//==============================================================================
//
// 行列4x4[Matrix4x4.cpp]
// Author : Yasuaki Yamashita : 2014/05/23
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
		// コンストラクタ
		//------------------------------------------------------------------------------
		Matrix4x4::Matrix4x4(
			float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24,
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44
			)
		{
			a._11 = f11, a._12 = f12, a._13 = f13, a._14 = f14;
			a._21 = f21, a._22 = f22, a._23 = f23, a._24 = f24;
			a._31 = f31, a._32 = f32, a._33 = f33, a._34 = f34;
			a._41 = f41, a._42 = f42, a._43 = f43, a._44 = f44;
		}

		Matrix4x4::Matrix4x4(const float * f)
		{

			a._11 = f[0], a._12 = f[1], a._13 = f[2], a._14 = f[3];
			a._21 = f[4], a._22 = f[5], a._23 = f[6], a._24 = f[7];
			a._31 = f[8], a._32 = f[9], a._33 = f[10], a._34 = f[11];
			a._41 = f[12], a._42 = f[13], a._43 = f[14], a._44 = f[15];
		}

		Matrix4x4::Matrix4x4(const Matrix4x4& f)
		{
			a._11 = f.m[0][0], a._12 = f.m[0][1], a._13 = f.m[0][2], a._14 = f.m[0][3];
			a._21 = f.m[1][0], a._22 = f.m[1][1], a._23 = f.m[1][2], a._24 = f.m[1][3];
			a._31 = f.m[2][0], a._32 = f.m[2][1], a._33 = f.m[2][2], a._34 = f.m[2][3];
			a._41 = f.m[3][0], a._42 = f.m[3][1], a._43 = f.m[3][2], a._44 = f.m[3][3];
		}

		//------------------------------------------------------------------------------
		// 行列の和
		//------------------------------------------------------------------------------
		Matrix4x4 Matrix4x4::operator + (const Matrix4x4& _a) const
		{
			return Matrix4x4(
				a._11 + _a.a._11, a._12 + _a.a._12, a._13 + _a.a._13, a._14 + _a.a._14,
				a._21 + _a.a._21, a._22 + _a.a._22, a._23 + _a.a._23, a._24 + _a.a._24,
				a._31 + _a.a._31, a._32 + _a.a._32, a._33 + _a.a._33, a._34 + _a.a._34,
				a._41 + _a.a._41, a._42 + _a.a._42, a._43 + _a.a._43, a._44 + _a.a._44
				);
		}

		//------------------------------------------------------------------------------
		// 行列の和
		//------------------------------------------------------------------------------
		Matrix4x4 Matrix4x4::operator * (float f) const
		{
			return Matrix4x4(
				a._11*f, a._12*f, a._13*f, a._14*f,
				a._21*f, a._22*f, a._23*f, a._24*f,
				a._31*f, a._32*f, a._33*f, a._34*f,
				a._41*f, a._42*f, a._43*f, a._44*f
				);
		}

		//------------------------------------------------------------------------------
		// 行列の除
		//------------------------------------------------------------------------------
		Matrix4x4 Matrix4x4::operator / (float _f) const
		{
			float f = 1.f / _f;
			return Matrix4x4(
				a._11*f, a._12*f, a._13*f, a._14*f,
				a._21*f, a._22*f, a._23*f, a._24*f,
				a._31*f, a._32*f, a._33*f, a._34*f,
				a._41*f, a._42*f, a._43*f, a._44*f
				);
		}


		//------------------------------------------------------------------------------
		// 行列の和
		//------------------------------------------------------------------------------
		Matrix4x4 Matrix4x4::operator * (const Matrix4x4& _a) const
		{
			Matrix4x4 s;

			s.a._11 = a._11 * _a.a._11 + a._12 * _a.a._21 + a._13 * _a.a._31 + a._14 * _a.a._41;
			s.a._12 = a._11 * _a.a._12 + a._12 * _a.a._22 + a._13 * _a.a._32 + a._14 * _a.a._42;
			s.a._13 = a._11 * _a.a._13 + a._12 * _a.a._23 + a._13 * _a.a._33 + a._14 * _a.a._43;
			s.a._14 = a._11 * _a.a._14 + a._12 * _a.a._24 + a._13 * _a.a._34 + a._14 * _a.a._44;

			s.a._21 = a._21 * _a.a._11 + a._22 * _a.a._21 + a._23 * _a.a._31 + a._24 * _a.a._41;
			s.a._22 = a._21 * _a.a._12 + a._22 * _a.a._22 + a._23 * _a.a._32 + a._24 * _a.a._42;
			s.a._23 = a._21 * _a.a._13 + a._22 * _a.a._23 + a._23 * _a.a._33 + a._24 * _a.a._43;
			s.a._24 = a._21 * _a.a._14 + a._22 * _a.a._24 + a._23 * _a.a._34 + a._24 * _a.a._44;

			s.a._31 = a._31 * _a.a._11 + a._32 * _a.a._21 + a._33 * _a.a._31 + a._34 * _a.a._41;
			s.a._32 = a._31 * _a.a._12 + a._32 * _a.a._22 + a._33 * _a.a._32 + a._34 * _a.a._42;
			s.a._33 = a._31 * _a.a._13 + a._32 * _a.a._23 + a._33 * _a.a._33 + a._34 * _a.a._43;
			s.a._34 = a._31 * _a.a._14 + a._32 * _a.a._24 + a._33 * _a.a._34 + a._34 * _a.a._44;

			s.a._41 = a._41 * _a.a._11 + a._42 * _a.a._21 + a._43 * _a.a._31 + a._44 * _a.a._41;
			s.a._42 = a._41 * _a.a._12 + a._42 * _a.a._22 + a._43 * _a.a._32 + a._44 * _a.a._42;
			s.a._43 = a._41 * _a.a._13 + a._42 * _a.a._23 + a._43 * _a.a._33 + a._44 * _a.a._43;
			s.a._44 = a._41 * _a.a._14 + a._42 * _a.a._24 + a._43 * _a.a._34 + a._44 * _a.a._44;

			return s;
		}

		//------------------------------------------------------------------------------
		// 単位行列
		//------------------------------------------------------------------------------
		void Matrix4x4::Identity(Matrix4x4* out)
		{
			out->a._11 = 1;
			out->a._12 = 0;
			out->a._13 = 0;
			out->a._14 = 0;
			out->a._21 = 0;
			out->a._22 = 1;
			out->a._23 = 0;
			out->a._24 = 0;
			out->a._31 = 0;
			out->a._32 = 0;
			out->a._33 = 1;
			out->a._34 = 0;
			out->a._41 = 0;
			out->a._42 = 0;
			out->a._43 = 0;
			out->a._44 = 1;
		}
		
		//------------------------------------------------------------------------------
		// 転置行列
		//------------------------------------------------------------------------------
		void Matrix4x4::Transposed(Matrix4x4* out, const Matrix4x4& in)
		{
			const Matrix4x4 tmp = in;
			out->a._11 = tmp.a._11;
			out->a._12 = tmp.a._21;
			out->a._13 = tmp.a._31;
			out->a._14 = tmp.a._41;
			out->a._21 = tmp.a._12;
			out->a._22 = tmp.a._22;
			out->a._23 = tmp.a._32;
			out->a._24 = tmp.a._42;
			out->a._31 = tmp.a._13;
			out->a._32 = tmp.a._23;
			out->a._33 = tmp.a._33;
			out->a._34 = tmp.a._43;
			out->a._41 = tmp.a._14;
			out->a._42 = tmp.a._24;
			out->a._43 = tmp.a._34;
			out->a._44 = tmp.a._44;
		}

		//------------------------------------------------------------------------------
		// 逆行列
		//------------------------------------------------------------------------------
		void Matrix4x4::Inverse(Matrix4x4* out, const Matrix4x4& in)
		{
			const Matrix4x4 tmp = in;
			const float det
				=tmp.n[0] *tmp.n[5] *tmp.n[10] *tmp.n[15] +tmp.n[0] *tmp.n[6] *tmp.n[11] *tmp.n[13] +tmp.n[0] *tmp.n[7] *tmp.n[9] *tmp.n[14]
				+tmp.n[1] *tmp.n[4] *tmp.n[11] *tmp.n[14] +tmp.n[1] *tmp.n[6] *tmp.n[8] *tmp.n[15] +tmp.n[1] *tmp.n[7] *tmp.n[10] *tmp.n[12]
				+tmp.n[2] *tmp.n[4] *tmp.n[9] *tmp.n[15] +tmp.n[2] *tmp.n[5] *tmp.n[11] *tmp.n[12] +tmp.n[2] *tmp.n[7] *tmp.n[8] *tmp.n[13]
				+tmp.n[3] *tmp.n[4] *tmp.n[10] *tmp.n[13] +tmp.n[3] *tmp.n[5] *tmp.n[8] *tmp.n[14] +tmp.n[3] *tmp.n[6] *tmp.n[9] *tmp.n[12]
				-tmp.n[0] *tmp.n[5] *tmp.n[11] *tmp.n[14] -tmp.n[0] *tmp.n[6] *tmp.n[9] *tmp.n[15] -tmp.n[0] *tmp.n[7] *tmp.n[10] *tmp.n[13]
				-tmp.n[1] *tmp.n[4] *tmp.n[10] *tmp.n[15] -tmp.n[1] *tmp.n[6] *tmp.n[11] *tmp.n[12] -tmp.n[1] *tmp.n[7] *tmp.n[8] *tmp.n[14]
				-tmp.n[2] *tmp.n[4] *tmp.n[11] *tmp.n[13] -tmp.n[2] *tmp.n[5] *tmp.n[8] *tmp.n[15] -tmp.n[2] *tmp.n[7] *tmp.n[9] *tmp.n[12]
				-tmp.n[3] *tmp.n[4] *tmp.n[9] *tmp.n[14] -tmp.n[3] *tmp.n[5] *tmp.n[10] *tmp.n[12] -tmp.n[3] *tmp.n[6] *tmp.n[8] *tmp.n[13];

			if (det != 0)
			{
				float inv_det = 1.0f / det;

				out->n[ 0] = inv_det * (tmp.n[ 5] *tmp.n[10] *tmp.n[15] +tmp.n[ 6] *tmp.n[11] *tmp.n[13] +tmp.n[ 7] *tmp.n[ 9] *tmp.n[14] -tmp.n[ 5] *tmp.n[11] *tmp.n[14] -tmp.n[ 6] *tmp.n[ 9] *tmp.n[15] -tmp.n[ 7] *tmp.n[10] *tmp.n[13]);
				out->n[ 1] = inv_det * (tmp.n[ 1] *tmp.n[11] *tmp.n[14] +tmp.n[ 2] *tmp.n[ 9] *tmp.n[15] +tmp.n[ 3] *tmp.n[10] *tmp.n[13] -tmp.n[ 1] *tmp.n[10] *tmp.n[15] -tmp.n[ 2] *tmp.n[11] *tmp.n[13] -tmp.n[ 3] *tmp.n[ 9] *tmp.n[14]);
				out->n[ 2] = inv_det * (tmp.n[ 1] *tmp.n[ 6] *tmp.n[15] +tmp.n[ 2] *tmp.n[ 7] *tmp.n[13] +tmp.n[ 3] *tmp.n[ 5] *tmp.n[14] -tmp.n[ 1] *tmp.n[ 7] *tmp.n[14] -tmp.n[ 2] *tmp.n[ 5] *tmp.n[15] -tmp.n[ 3] *tmp.n[ 6] *tmp.n[13]);
				out->n[ 3] = inv_det * (tmp.n[ 1] *tmp.n[ 7] *tmp.n[10] +tmp.n[ 2] *tmp.n[ 5] *tmp.n[11] +tmp.n[ 3] *tmp.n[ 6] *tmp.n[ 9] -tmp.n[ 1] *tmp.n[ 6] *tmp.n[11] -tmp.n[ 2] *tmp.n[ 7] *tmp.n[ 9] -tmp.n[ 3] *tmp.n[ 5] *tmp.n[10]);
 
				out->n[ 4] = inv_det * (tmp.n[ 4] *tmp.n[11] *tmp.n[14] +tmp.n[ 6] *tmp.n[ 8] *tmp.n[15] +tmp.n[ 7] *tmp.n[10] *tmp.n[12] -tmp.n[ 4] *tmp.n[10] *tmp.n[15] -tmp.n[ 6] *tmp.n[11] *tmp.n[12] -tmp.n[ 7] *tmp.n[ 8] *tmp.n[14]);
				out->n[ 5] = inv_det * (tmp.n[ 0] *tmp.n[10] *tmp.n[15] +tmp.n[ 2] *tmp.n[11] *tmp.n[12] +tmp.n[ 3] *tmp.n[ 8] *tmp.n[14] -tmp.n[ 0] *tmp.n[11] *tmp.n[14] -tmp.n[ 2] *tmp.n[ 8] *tmp.n[15] -tmp.n[ 3] *tmp.n[10] *tmp.n[12]);
				out->n[ 6] = inv_det * (tmp.n[ 0] *tmp.n[ 7] *tmp.n[14] +tmp.n[ 2] *tmp.n[ 4] *tmp.n[15] +tmp.n[ 3] *tmp.n[ 6] *tmp.n[12] -tmp.n[ 0] *tmp.n[ 6] *tmp.n[15] -tmp.n[ 2] *tmp.n[ 7] *tmp.n[12] -tmp.n[ 3] *tmp.n[ 4] *tmp.n[14]);
				out->n[ 7] = inv_det * (tmp.n[ 0] *tmp.n[ 6] *tmp.n[11] +tmp.n[ 2] *tmp.n[ 7] *tmp.n[ 8] +tmp.n[ 3] *tmp.n[ 4] *tmp.n[10] -tmp.n[ 0] *tmp.n[ 7] *tmp.n[10] -tmp.n[ 2] *tmp.n[ 4] *tmp.n[11] -tmp.n[ 3] *tmp.n[ 6] *tmp.n[ 8]);

				out->n[ 8] = inv_det * (tmp.n[ 4] *tmp.n[ 9] *tmp.n[15] +tmp.n[ 5] *tmp.n[11] *tmp.n[12] +tmp.n[ 7] *tmp.n[ 8] *tmp.n[13] -tmp.n[ 4] *tmp.n[11] *tmp.n[13] -tmp.n[ 5] *tmp.n[ 8] *tmp.n[15] -tmp.n[ 7] *tmp.n[ 9] *tmp.n[12]);
				out->n[ 9] = inv_det * (tmp.n[ 0] *tmp.n[11] *tmp.n[13] +tmp.n[ 1] *tmp.n[ 8] *tmp.n[15] +tmp.n[ 3] *tmp.n[ 9] *tmp.n[12] -tmp.n[ 0] *tmp.n[ 9] *tmp.n[15] -tmp.n[ 1] *tmp.n[11] *tmp.n[12] -tmp.n[ 3] *tmp.n[ 8] *tmp.n[13]);
				out->n[10] = inv_det * (tmp.n[ 0] *tmp.n[ 5] *tmp.n[15] +tmp.n[ 1] *tmp.n[ 7] *tmp.n[12] +tmp.n[ 3] *tmp.n[ 4] *tmp.n[13] -tmp.n[ 0] *tmp.n[ 7] *tmp.n[13] -tmp.n[ 1] *tmp.n[ 4] *tmp.n[15] -tmp.n[ 3] *tmp.n[ 5] *tmp.n[12]);
				out->n[11] = inv_det * (tmp.n[ 0] *tmp.n[ 7] *tmp.n[ 9] +tmp.n[ 1] *tmp.n[ 4] *tmp.n[11] +tmp.n[ 3] *tmp.n[ 5] *tmp.n[ 8] -tmp.n[ 0] *tmp.n[ 5] *tmp.n[11] -tmp.n[ 1] *tmp.n[ 7] *tmp.n[ 8] -tmp.n[ 3] *tmp.n[ 4] *tmp.n[ 9]);

				out->n[12] = inv_det * (tmp.n[ 4] *tmp.n[10] *tmp.n[13] +tmp.n[ 5] *tmp.n[ 8] *tmp.n[14] +tmp.n[ 6] *tmp.n[ 9] *tmp.n[12] -tmp.n[ 4] *tmp.n[ 9] *tmp.n[14] -tmp.n[ 5] *tmp.n[10] *tmp.n[12] -tmp.n[ 6] *tmp.n[ 8] *tmp.n[13]);
				out->n[13] = inv_det * (tmp.n[ 0] *tmp.n[ 9] *tmp.n[14] +tmp.n[ 1] *tmp.n[10] *tmp.n[12] +tmp.n[ 2] *tmp.n[ 8] *tmp.n[13] -tmp.n[ 0] *tmp.n[10] *tmp.n[13] -tmp.n[ 1] *tmp.n[ 8] *tmp.n[14] -tmp.n[ 2] *tmp.n[ 9] *tmp.n[12]);
				out->n[14] = inv_det * (tmp.n[ 0] *tmp.n[ 6] *tmp.n[13] +tmp.n[ 1] *tmp.n[ 4] *tmp.n[14] +tmp.n[ 2] *tmp.n[ 5] *tmp.n[12] -tmp.n[ 0] *tmp.n[ 5] *tmp.n[14] -tmp.n[ 1] *tmp.n[ 6] *tmp.n[12] -tmp.n[ 2] *tmp.n[ 4] *tmp.n[13]);
				out->n[15] = inv_det * (tmp.n[ 0] *tmp.n[ 5] *tmp.n[10] +tmp.n[ 1] *tmp.n[ 6] *tmp.n[ 8] +tmp.n[ 2] *tmp.n[ 4] *tmp.n[ 9] -tmp.n[ 0] *tmp.n[ 6] *tmp.n[ 9] -tmp.n[ 1] *tmp.n[ 4] *tmp.n[10] -tmp.n[ 2] *tmp.n[ 5] *tmp.n[ 8]);
			}
		}

		//------------------------------------------------------------------------------
		// 単位行列
		//------------------------------------------------------------------------------
		void Matrix4x4::Identity(void)
		{
			Identity(this);
		}

		//------------------------------------------------------------------------------
		// 転置行列
		//------------------------------------------------------------------------------
		void Matrix4x4::Transposed(void)
		{
			Transposed(this, *this);
		}

		//------------------------------------------------------------------------------
		// 逆行列
		//------------------------------------------------------------------------------
		void Matrix4x4::Inverse(void)
		{
			Inverse(this, *this);
		}

		//------------------------------------------------------------------------------
		// 転置行列
		//------------------------------------------------------------------------------
		void Matrix4x4::Transposed(Matrix4x4* inOut)
		{
			Transposed(inOut, *inOut);
		}

		//------------------------------------------------------------------------------
		// 逆行列
		//------------------------------------------------------------------------------
		void Matrix4x4::Inverse(Matrix4x4* inOut)
		{
			Inverse(inOut, *inOut);
		}
	}
}
//EOF