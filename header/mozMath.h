//==============================================================================
//
// 数学ライブラリ[mozMath.h]
// Author : Yasuaki Yamashita : 2014/06/26
//
//==============================================================================

#pragma once

#ifndef _MOZMATH_H_
#define _MOZMATH_H_

#ifdef _DEBUG
#pragma comment(lib, "./lib/MozMath_d.lib")
#else
#pragma comment(lib, "./lib/MozMath.lib")
#endif

namespace moz
{
	namespace math
	{

		//==============================================================================
		// struct
		//------------------------------------------------------------------------------
		// vector2d
		struct _v2 { float x, y; };
		// vector3d
		struct _v3 { float x, y, z; };
		// vector4d
		struct _v4 { float x, y, z, w; };

		// matrix4x4
		struct _4x4
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};

		//==============================================================================
		// const
		//------------------------------------------------------------------------------
		const float kPI       = 3.141592654f;
		const float kTWOPI    = 3.141592654f * 2.f; 
		const float kRCPTWOPI = 0.159154943f;
		#define ToRadian( deg ) ((deg) * (kPI / 180.f))
		#define ToDegree( rad ) ((rad) * (180.f / kPI))

		//==============================================================================
		// degree
		//------------------------------------------------------------------------------
		float NormalizeAngle(float Angle)
		{
			long ofs = (*(long*)&Angle & 0x80000000) | 0x3F000000;
			return (Angle - ((int)(Angle * kRCPTWOPI + *(float*)&ofs) * kTWOPI));
		}


		//==============================================================================
		// 2DVector
		//------------------------------------------------------------------------------
		struct Vector2D
		{
		public:
			Vector2D(){};
			Vector2D(float _x, float _y);
			Vector2D operator *(float) const;
			Vector2D operator /(float) const;
			Vector2D operator +(const Vector2D&) const;
			Vector2D operator -(const Vector2D&) const;
			Vector2D operator +() const;
			Vector2D operator -() const;
			Vector2D& operator +=(const Vector2D&);
			Vector2D& operator -=(const Vector2D&);
			Vector2D& operator *=(float);
			Vector2D& operator /=(float);

		public:
			// 内積
			inline float Dot(const Vector2D&);
			// 外積	
			inline float Cross(const Vector2D&);
			// 正規化
			inline void Normalize(void);
			// 長さ取得
			inline float GetLength(void);

		public:
			union
			{
				_v2 v;
				float m[2];
			};
		};

		//==============================================================================
		// 3DVector
		//------------------------------------------------------------------------------
		struct Vector3D
		{
		public:
			Vector3D(){};
			Vector3D(float _x, float _y, float _z);
			Vector3D operator *(float) const;
			Vector3D operator /(float) const;
			Vector3D operator +(const Vector3D&) const;
			Vector3D operator -(const Vector3D&) const;
			Vector3D operator +() const;
			Vector3D operator -() const;
			Vector3D& operator +=(const Vector3D&);
			Vector3D& operator -=(const Vector3D&);
			Vector3D& operator *=(float);
			Vector3D& operator /=(float);

		public:
			// 内積
			inline float Dot(const Vector3D&);
			// 外積	
			inline Vector3D Cross(const Vector3D&);
			// 正規化
			inline void Normalize(void);
			// 長さ取得
			inline float GetLength(void);

		public:
			union
			{
				_v3 v;
				float m[3];
			};
		};

		//==============================================================================
		// 4DVector
		//------------------------------------------------------------------------------
		struct Vector4D
		{
		public:
			Vector4D(){};
			Vector4D(float _x, float _y, float _z, float _w);
			Vector4D operator *(float) const;
			Vector4D operator /(float) const;
			Vector4D operator +(const Vector4D&) const;
			Vector4D operator -(const Vector4D&) const;
			Vector4D operator +() const;
			Vector4D operator -() const;
			Vector4D& operator +=(const Vector4D&);
			Vector4D& operator -=(const Vector4D&);
			Vector4D& operator *=(float);
			Vector4D& operator /=(float);

		public:
			// 内積
			//float Dot(const Vector4D&);
			// 外積	
			//Vector4D& Cross(const Vector4D&);
			// 正規化
			void Normalize(void);
			// 長さ取得
			float GetLength(void);

		public:
			union
			{
				_v4 v;
				float m[4];
			};
		};

		//==============================================================================
		// 4x4Matrix
		//------------------------------------------------------------------------------
		struct Matrix4x4
		{
		public:
			Matrix4x4(){};
			Matrix4x4(
				float f11, float f12, float f13, float f14,
				float f21, float f22, float f23, float f24,
				float f31, float f32, float f33, float f34,
				float f41, float f42, float f43, float f44
				);

			Matrix4x4(const float *);
			Matrix4x4(const Matrix4x4&);

			// 計算式
			Matrix4x4& operator *= (const Matrix4x4&);
			Matrix4x4& operator += (const Matrix4x4&);
			Matrix4x4& operator -= (const Matrix4x4&);
			Matrix4x4& operator *= (float);
			Matrix4x4& operator /= (float);


			Matrix4x4 operator + (const Matrix4x4&) const;
			Matrix4x4 operator - (const Matrix4x4&) const;
			Matrix4x4 operator * (float) const;
			Matrix4x4 operator / (float) const;
			Matrix4x4 operator * (const Matrix4x4&) const;

			bool operator == (const Matrix4x4&) const;
			bool operator != (const Matrix4x4&) const;
			
		public:
			// 単位行列
			void Identity(void);
			// 転置行列

			// 逆行列
			void Inverse(){};


		public:
			union
			{
				_4x4  a;
				float m[4][4];
				float n[16];
			};
		};

	}
}
#endif
//EOF