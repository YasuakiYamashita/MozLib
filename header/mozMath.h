//==============================================================================
//
// ”Šwƒ‰ƒCƒuƒ‰ƒŠ[mozMath.h]
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
		// ƒ†[ƒeƒBƒŠƒeƒB
		//------------------------------------------------------------------------------
		float NormalizeAngle(float Angle);		// ³‹K‰»

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
			// “àÏ
			inline float Dot(const Vector2D&);
			// ŠOÏ	
			inline float Cross(const Vector2D&);
			// ³‹K‰»
			inline void Normalize(void);
			// ’·‚³æ“¾
			inline float GetLength(void);

		public:
			// “àÏ
			static inline float Dot(const Vector2D&, const Vector2D&);
			// ŠOÏ	
			static inline float Cross(const Vector2D&, const Vector2D&);
			// ³‹K‰»
			static inline void Normalize(Vector2D* out, const Vector2D& in);
			// ’·‚³æ“¾
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
			// “àÏ
			inline float Dot(const Vector3D&);
			// ŠOÏ	
			inline Vector3D Cross(const Vector3D&);
			// ³‹K‰»
			inline void Normalize(void);
			// ’·‚³æ“¾
			inline float GetLength(void);

		public:
			// “àÏ
			static inline float Dot( const Vector3D&, const Vector3D&);
			// ŠOÏ
			static inline void Cross(Vector3D* out, const Vector3D&, const Vector3D&);
			// ³‹K‰»
			static inline void Normalize(Vector3D* out, const Vector3D& in);
			// ’·‚³æ“¾
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
			// “àÏ
			//float Dot(const Vector4D&);
			// ŠOÏ	
			//Vector4D& Cross(const Vector4D&);
			// ³‹K‰»
			inline void Normalize(void);
			// ’·‚³æ“¾
			inline float GetLength(void);

		public:
			// ³‹K‰»
			static inline float GetLength(const Vector4D&);
			// ’·‚³æ“¾
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

			// ŒvZ®
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
			// ’PˆÊs—ñ
			inline void Identity(void);
			// “]’us—ñ
			inline void Transposed(void);
			// ‹ts—ñ
			inline void Inverse(void);

		public:
			// ’PˆÊs—ñ
			static inline void Identity(Matrix4x4* out);
			// “]’us—ñ
			static inline void Transposed(Matrix4x4* out, const Matrix4x4& in);
			// ‹ts—ñ
			static inline void Inverse(Matrix4x4* out, const Matrix4x4& in);


			// “]’us—ñ
			static inline void Transposed(Matrix4x4* inOut);
			// ‹ts—ñ
			static inline void Inverse(Matrix4x4* inOut);


		public:

			// Y²‰ñ“]‚Ìs—ñì¬
			static inline void CreateYawMatrix(Matrix4x4* mat, float rot);
			// X²‰ñ“]‚Ìs—ñì¬
			static inline void CreatePitchMatrix(Matrix4x4* mat, float rot);
			// Z²‰ñ“]‚Ìs—ñì¬
			static inline void CreateRollMatrix(Matrix4x4* mat, float rot);
			// ‘S‚Ä‚Ì²‰ñ“]‚Ìs—ñì¬
			static inline void CreateYawPitchRollMatrix(Matrix4x4* mat, float yaw, float pitch, float roll);


			// Y²‰ñ“]
			static inline void RotationYaw(Matrix4x4* out, const Matrix4x4& in, float rot);
			// X²‰ñ“]
			static inline void RotationPitch(Matrix4x4* out, const Matrix4x4& in, float rot);
			// Z²‰ñ“]
			static inline void RotationRoll(Matrix4x4* out, const Matrix4x4& in, float rot);
			// ‘S‚Ä‚Ì‰ñ“]
			static inline void RotationYawPitchRoll(Matrix4x4* out, const Matrix4x4& in, float yaw, float pitch, float roll);

			// Y²‰ñ“]
			inline void RotationYaw(float rot);
			// X²‰ñ“]
			inline void RotationPitch(float rot);
			// Z²‰ñ“]
			inline void RotationRoll(float rot);
			// ‘S‚Ä‚Ì‰ñ“]
			inline void RotationYawPitchRoll(float yaw, float pitch, float roll);

			// ƒXƒP[ƒ‹
			static inline void Scaling(Matrix4x4* out, const Matrix4x4& in, float x, float y, float z);
			inline void Scaling(float x, float y, float z);

			// •½sˆÚ“®
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