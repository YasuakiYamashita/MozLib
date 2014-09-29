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
#include <vector>

//==============================================================================
// ポリゴン表示用
//------------------------------------------------------------------------------
namespace moz
{
	namespace DirectX
	{
		// 前方宣言
		class PolygonManager;
		using namespace moz::math;

		// 3D用頂点フォーマット
		static const D3DVERTEXELEMENT9 Vtx3dDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 2, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};


		// 3D用頂点フォーマット
		static const D3DVERTEXELEMENT9 Vtx3dDecl2[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		// 2D用頂点フォーマット
		static const D3DVERTEXELEMENT9 Vtx2dDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		// 静的確保
		static const int k3DMaxBuffer = 40000;	// 3Dポリゴン
		static const int k2DMaxBuffer = 40000;	// 2Dポリゴン


		static const int SHADOW_MAP_SIZE = 1024;

		// ロックに使う時のバッファ
		struct _2DLOCKBUFF {
			Vector3D*		vtx;
			Color*			col;
			Vector2D*		tex;
			unsigned int	num;
		};
		// ロックに使う時のバッファ
		struct _3DLOCKBUFF {
			Vector3D*		vtx;
			Vector3D*		nor;
			Color*			col;
			Vector2D*		tex;
			unsigned int	num;
		};

		//==============================================================================
		// ポリゴンコンテナ
		//------------------------------------------------------------------------------
		class PolygonContainer
		{
		public:
			PolygonContainer()
				: m_DethFlag(false)
				, m_rot(0, 0, 0)
				, m_pos(0, 0, 0)
				, m_scl(1, 1, 1)
				, m_tex(nullptr)
				, m_vtxNum(0)
				, m_col(Color(1, 1, 1, 1))
				, m_manager(nullptr)
				, m_indexBuff(nullptr)
			{};
			virtual ~PolygonContainer();

			virtual void DrawUpdate() = 0;
			virtual void Update() = 0;
			inline virtual void Draw(const LPDIRECT3DDEVICE9& device, unsigned int startvtx, unsigned int vtxnum);

			virtual void SetVtx(){ ASSERT(false, "error"); }

			Vector3D& GetRot(void) { return m_rot; }
			Vector3D& GetPos(void) { return m_pos; }
			Vector3D& GetScl(void) { return m_scl; }
			Color& GetCol(void) { return m_col; }

			TexContainer*& GetTex(void) { return m_tex; }

			unsigned int GetVtxNum(void) { return m_vtxNum; }
			bool GetDethFlag(void){ return m_DethFlag; }
			void SetManager(PolygonManager* p){ m_manager = p; }

			LPDIRECT3DINDEXBUFFER9& GetIndexBuff(void){ return m_indexBuff; }

		protected:
			Vector3D m_rot;
			Vector3D m_pos;
			Vector3D m_scl;
			Color    m_col;
			PolygonManager* m_manager;
			LPDIRECT3DINDEXBUFFER9 m_indexBuff;

			void SetVtxNum(unsigned int num) { m_vtxNum = num; }

		private:
			unsigned int m_vtxNum;
			TexContainer *m_tex;
			bool m_DethFlag;	// 削除フラグ

		};

		//==============================================================================
		// 3Dポリゴン
		//------------------------------------------------------------------------------
		class Polygon3D : public PolygonContainer
		{
		public:
			Polygon3D(const Vector2D& size);
			virtual ~Polygon3D();

			virtual void SetVtx(const _3DLOCKBUFF* buff);
			virtual void DrawUpdate(void);
			virtual void Update(void);

			const D3DXMATRIX& GetWorldMtx(void){ return m_mtxWorld; }

		protected:
			Vector2D m_size;
			D3DXMATRIX m_mtxWorld;

		};

		//==============================================================================
		// 2Dポリゴン
		//------------------------------------------------------------------------------
		class Polygon2D : public PolygonContainer
		{
		public:
			Polygon2D(const Vector2D& size);
			virtual ~Polygon2D();

			virtual void SetVtx(const _2DLOCKBUFF* buff);
			virtual void DrawUpdate(void);
			virtual void Update(void);

		protected:
			Vector2D m_size;

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

			// 作成とか
			Polygon2D* Create2D(const Vector2D& size = Vector2D(100.f, 100.f), const Color& col = Color(1, 1, 1, 1));
			
			// 3Dポリゴン作成
			template<class _T, class... Args> _T* Create3D(const Args&... args)
			{
				_T* buf = new _T(args...);

				buf->SetManager(this);

				if (m_3DusingNum + buf->GetVtxNum() > k3DMaxBuffer)
				{
					ASSERT(false, "3Dバッファオーバー");
					SAFE_DELETE(buf);
					return nullptr;
				}

				Lock3D(m_3DusingNum, buf->GetVtxNum());
				buf->SetVtx(&m_3DLockBuff);
				Unlock3D();

				m_3DusingNum += buf->GetVtxNum();
				m_3DPolygonList.push_back(buf);

				return buf;
			}

			const D3DXMATRIX& GetViewMtx(void){ return m_mtxView; }
			const D3DXMATRIX& GetProjMtx(void){ return m_mtxProj; }

			// Index
			LPDIRECT3DINDEXBUFFER9 CreateIndex(unsigned int IndexNum, const WORD* Index);

			D3DXVECTOR3& GetCamPos(void){ return m_posCameraP; }

		private:
			//描画
			void Draw2DList(void);
			void Draw3DList(void);

			// 初期化
			void Init2DVtx(void);
			void Init3DVtx(void);

			// 2Dバッファロック
			void Lock2D(unsigned int vtxoffset, unsigned int vtxnum);
			void Unlock2D(void);
			// 3Dバッファロック
			void Lock3D(unsigned int vtxoffset, unsigned int vtxnum);
			void Unlock3D(void);

			//===================================
			// ポリゴンリスト
			std::vector<Polygon2D*> m_2DPolygonList;
			std::vector<Polygon3D*> m_3DPolygonList;

			//===================================
			// 2D使用
			// 描画用バッファ 
			struct {
				IDirect3DVertexBuffer9* _vtx;
				IDirect3DVertexBuffer9* _col;
				IDirect3DVertexBuffer9* _tex;
			} m_2DVtxBuff;	

			// 使用するバッファ
			_2DLOCKBUFF m_2DLockBuff;				
			unsigned int m_2DusingNum;	// 使用した数
			LPD3DXEFFECT m_2DEffect;	// シェーダ用
			struct {
				D3DXHANDLE proj;
				D3DXHANDLE world;
				D3DXHANDLE tex;
			} m_2DHandle;				// ハンドル

			//===================================
			// 3D使用
			// 描画用バッファ 
			struct {
				IDirect3DVertexBuffer9* _vtx;
				IDirect3DVertexBuffer9* _nor;
				IDirect3DVertexBuffer9* _col;
				IDirect3DVertexBuffer9* _tex;
			} m_3DVtxBuff;
			// 使用するバッファ
			_3DLOCKBUFF m_3DLockBuff;

			// バッファ
			IDirect3DVertexBuffer9* m_3DVtxBuff2;

			unsigned int m_3DusingNum;
			struct {
				D3DXHANDLE matWVP;
				D3DXHANDLE tex;
			} m_3DHandle;
			LPD3DXEFFECT m_3DEffect;	// シェーダ用

			// カメラ用
			D3DXVECTOR3 m_posCameraP;
			D3DXVECTOR3 m_posCameraR;
			D3DXVECTOR3 m_vecCameraU;

			// ライト位置
			D3DXVECTOR3 m_LighPos;

			//===================================
			// 頂点宣言
			LPDIRECT3DVERTEXDECLARATION9 m_p3DDec;
			LPDIRECT3DVERTEXDECLARATION9 m_p3DDec2;
			LPDIRECT3DVERTEXDECLARATION9 m_p2DDec;
			
			// 描画用マトリックス
			D3DXMATRIX m_mtxView;
			D3DXMATRIX m_mtxProj;
			D3DXMATRIX m_mLightVP;

			// 影用
			LPDIRECT3DSURFACE9		m_pShadowMapZ;		// 深度バッファ
			LPDIRECT3DTEXTURE9		m_pShadowMap;		// テクスチャ
			LPDIRECT3DSURFACE9		m_pShadowMapSurf;	// サーフェス
			LPDIRECT3DTEXTURE9		m_pEdgeMap;			// テクスチャ
			LPDIRECT3DSURFACE9		m_pEdgeMapSurf;		// サーフェス
			LPDIRECT3DTEXTURE9		m_pSoftMap[2];		// テクスチャ
			LPDIRECT3DSURFACE9		m_pSoftMapSurf[2];	// サーフェス

			DirectX* m_pDirectX;
		};

		
	}
}

#endif
//EOF