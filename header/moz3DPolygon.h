//==============================================================================
//
// 3Dポリゴン作成[moz3DPolygon.h]
// Author : Yasuaki Yamashita : 2014/09/24
//
//==============================================================================

#pragma once

#ifndef _MOZ3DPOLYGON_H_
#define _MOZ3DPOLYGON_H_

//==============================================================================
// include
//------------------------------------------------------------------------------
#include <mozPolygon.h>

namespace moz
{ 
	namespace DirectX
	{
		//==============================================================================
		// ビルボード用
		//------------------------------------------------------------------------------
		class PolygonBillboard : public Polygon3D
		{
		public:
			PolygonBillboard(const Vector2D& size);
			virtual ~PolygonBillboard();
			
			virtual void SetVtx(const _3DLOCKBUFF* buff);
			virtual void DrawUpdate(void);
			virtual void Update(void);
		};

		//==============================================================================
		// Index有りの場合
		//------------------------------------------------------------------------------
		class PolygonIndex : public Polygon3D
		{
		public:
			PolygonIndex(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
			virtual ~PolygonIndex();

			inline virtual void Draw(const LPDIRECT3DDEVICE9& device, unsigned int startvtx, unsigned int vtxnum);
			virtual void SetVtx(const _3DLOCKBUFF* buff);
			virtual void Update(void);

		protected:
			// 各面の頂点設定
			void _InitVertexXY(const _3DLOCKBUFF* buff);
			void _InitVertexXZ(const _3DLOCKBUFF* buff);
			void _InitVertexZY(const _3DLOCKBUFF* buff);
			void _InitVertexXYReverse(const _3DLOCKBUFF* buff);
			void _InitVertexXZReverse(const _3DLOCKBUFF* buff);
			void _InitVertexZYReverse(const _3DLOCKBUFF* buff);


		protected:
			// ブロックスの数
			unsigned int m_nNumBlockX, m_nNumBlockY;

			// ブロックあたりの大きさ
			float m_fSizeBlockX, m_fSizeBlockY;

			// 必要個数
			unsigned int m_nNumVertexIndex, m_nNumPolygon;

		};

	}
}
#endif
//EOF