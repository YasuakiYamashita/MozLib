//==============================================================================
//
// ポリゴン用[mozPolygon.h]
// Author : Yasuaki Yamashita : 2014/08/22
//
//==============================================================================

#pragma once

#ifndef _MOZPOLYGON_H_
#define _MOZPOLYGON_H_

#include <mozDirectX.h>
#include <mozMath.h>


using namespace moz::math;

//==============================================================================
// ポリゴン表示用
//------------------------------------------------------------------------------
namespace moz
{

	namespace DirectX
	{
		// 3D用頂点フォーマット
		static const D3DVERTEXELEMENT9 Vtx3dDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 2, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		// 2D用頂点フォーマット
		static const D3DVERTEXELEMENT9 Vtx2dDecl[] =
		{
			{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
			{ 2, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		// 静的確保
		static const int k3DPolygonNum = 10000;	// 3Dポリゴン
		static const int k2DPolygonNum = 100;	// 2Dポリゴン

		//==============================================================================
		// ポリゴンコンテナ
		//------------------------------------------------------------------------------
		class DrawList
		{
		public:
			DrawList(){};
			virtual ~DrawList();
			
			virtual void Draw() = 0;
			virtual void Update() = 0;

		protected:
			IDirect3DVertexBuffer9* m_vtxDecl;
			Vector3D m_rot;
			Vector3D m_pos;
			Vector3D m_scl;
		};

		//==============================================================================
		// 3Dポリゴン
		//------------------------------------------------------------------------------
		class DrawList3D
		{
		public:
			DrawList3D();
			virtual ~DrawList3D();

			virtual void Draw(void);
			virtual void Update(void);

		protected:

		};

		//==============================================================================
		// 2Dポリゴン
		//------------------------------------------------------------------------------
		class DrawList2D
		{
		public:
			DrawList2D();
			virtual ~DrawList2D();

			virtual void Draw(void);
			virtual void Update(void);
		};

		//==============================================================================
		// 全てのポリゴンマネージャ
		//------------------------------------------------------------------------------
		class PolygonManager
		{
		public:
			PolygonManager(DirectX *);
			virtual ~PolygonManager();

			// いつもの
			void Update(void);
			void Draw(void);

		private:
			std::list<DrawList*> m_2DPolygonList;
			std::list<DrawList*> m_3DPolygonList;
			std::list<DrawList*> m_EffectPolygonList;

			// 
			IDirect3DVertexBuffer9* m_2Dvtx;
			IDirect3DVertexBuffer9* m_3Dvtx;
			IDirect3DVertexBuffer9* m_Effectvtx;

			LPDIRECT3DVERTEXDECLARATION9 m_p3DDec;
			LPDIRECT3DVERTEXDECLARATION9 m_p2DDec;

			DirectX* m_pDirectX;
		};

		
	}
}

#endif
//EOF