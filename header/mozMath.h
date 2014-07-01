//==============================================================================
//
// ���w���C�u����[mozMath.h]
// Author : Yasuaki Yamashita : 2014/06/26
//
//==============================================================================

#pragma once

#ifndef _MOZMATH_H_
#define _MOZMATH_H_

#ifdef _DEBUG
#pragma (lib, "lib/mozMath_d.lib")
#else
#pragma (lib, "lib/mozMath.lib")
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
		// color
		struct col { float r, g, b, a; };

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
		static const float kPI       = 3.141592654f;
		static const float kTWOPI    = 3.141592654f * 2.f; 
		static const float kRCPTWOPI = 0.159154943f;
		#define ToRadian( deg ) ((deg) * (kPI / 180.f))
		#define ToDegree( rad ) ((rad) * (180.f / kPI))
		#define POW2(x) ((x)*(x))

		//==============================================================================
		// ���[�e�B���e�B
		//------------------------------------------------------------------------------
		float NormalizeAngle(float Angle);		// ���K��

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
			// ����
			inline float Dot(const Vector2D&);
			// �O��	
			inline float Cross(const Vector2D&);
			// ���K��
			inline void Normalize(void);
			// �����擾
			inline float GetLength(void);

		public:
			// ����
			static inline float Dot(const Vector2D&, const Vector2D&);
			// �O��	
			static inline float Cross(const Vector2D&, const Vector2D&);
			// ���K��
			static inline void Normalize(Vector2D* out, const Vector2D& in);
			// �����擾
			static inline float GetLength(const Vector2D& in);

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
			// ����
			inline float Dot(const Vector3D&);
			// �O��	
			inline Vector3D Cross(const Vector3D&);
			// ���K��
			inline void Normalize(void);
			// �����擾
			inline float GetLength(void);

		public:
			// ����
			static inline float Dot( const Vector3D&, const Vector3D&);
			// �O��
			static inline void Cross(Vector3D* out, const Vector3D&, const Vector3D&);
			// ���K��
			static inline void Normalize(Vector3D* out, const Vector3D& in);
			// �����擾
			static inline float GetLength(const Vector3D& in);

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
			// ����
			//float Dot(const Vector4D&);
			// �O��	
			//Vector4D& Cross(const Vector4D&);
			// ���K��
			inline void Normalize(void);
			// �����擾
			inline float GetLength(void);

		public:
			// ���K��
			static inline float GetLength(const Vector4D&);
			// �����擾
			static inline void Normalize(Vector4D* out, const Vector4D& in);


		public:
			union
			{
				_v4 v;
				float m[4];
			};
		};

		//==============================================================================
		// Color
		//------------------------------------------------------------------------------
		struct Color
		{
		public:
			Color(){};
			Color(float r, float g, float b, float a);

		public:
			Color& operator+= (const Color&);
			Color& operator-= (const Color&);
			Color& operator*= (float);
			Color& operator/= (float);

			Color operator + () const;
			Color operator - () const;

			Color operator + (const Color&) const;
			Color operator - (const Color&) const;
			Color operator * (float) const;
			Color operator / (float) const;

			bool operator == (const Color&) const;
			bool operator != (const Color&) const;

		public:
			union
			{
				col c;
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

			// �v�Z��
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
			// �P�ʍs��
			inline void Identity(void);
			// �]�u�s��
			inline void Transposed(void);
			// �t�s��
			inline void Inverse(void);

		public:
			// �P�ʍs��
			static inline void Identity(Matrix4x4* out);
			// �]�u�s��
			static inline void Transposed(Matrix4x4* out, const Matrix4x4& in);
			// �t�s��
			static inline void Inverse(Matrix4x4* out, const Matrix4x4& in);


			// �]�u�s��
			static inline void Transposed(Matrix4x4* inOut);
			// �t�s��
			static inline void Inverse(Matrix4x4* inOut);


		public:

			// Y����]�̍s��쐬
			static inline void CreateYawMatrix(Matrix4x4* mat, float rot);
			// X����]�̍s��쐬
			static inline void CreatePitchMatrix(Matrix4x4* mat, float rot);
			// Z����]�̍s��쐬
			static inline void CreateRollMatrix(Matrix4x4* mat, float rot);
			// �S�Ă̎���]�̍s��쐬
			static inline void CreateYawPitchRollMatrix(Matrix4x4* mat, float yaw, float pitch, float roll);


			// Y����]
			static inline void RotationYaw(Matrix4x4* out, const Matrix4x4& in, float rot);
			// X����]
			static inline void RotationPitch(Matrix4x4* out, const Matrix4x4& in, float rot);
			// Z����]
			static inline void RotationRoll(Matrix4x4* out, const Matrix4x4& in, float rot);
			// �S�Ẳ�]
			static inline void RotationYawPitchRoll(Matrix4x4* out, const Matrix4x4& in, float yaw, float pitch, float roll);

			// Y����]
			inline void RotationYaw(float rot);
			// X����]
			inline void RotationPitch(float rot);
			// Z����]
			inline void RotationRoll(float rot);
			// �S�Ẳ�]
			inline void RotationYawPitchRoll(float yaw, float pitch, float roll);

			// �X�P�[��
			static inline void Scaling(Matrix4x4* out, const Matrix4x4& in, float x, float y, float z);
			inline void Scaling(float x, float y, float z);

			// ���s�ړ�
			static inline void Translation(Matrix4x4* out, const Matrix4x4& in, float x, float y, float z);
			inline void Translation(float x, float y, float z);
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