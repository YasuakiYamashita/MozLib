//==============================================================================
//
// �|���S���p[mozPolygon.h]
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
// �|���S���\���p
//------------------------------------------------------------------------------
namespace moz
{
	namespace DirectX
	{
		// �O���錾
		class PolygonManager;
		using namespace moz::math;

		// 3D�p���_�t�H�[�}�b�g
		static const D3DVERTEXELEMENT9 Vtx3dDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 2, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};


		// 3D�p���_�t�H�[�}�b�g
		static const D3DVERTEXELEMENT9 Vtx3dDecl2[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		// 2D�p���_�t�H�[�}�b�g
		static const D3DVERTEXELEMENT9 Vtx2dDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		// �ÓI�m��
		static const int k3DMaxBuffer = 40000;	// 3D�|���S��
		static const int k2DMaxBuffer = 40000;	// 2D�|���S��


		static const int SHADOW_MAP_SIZE = 1024;

		// ���b�N�Ɏg�����̃o�b�t�@
		struct _2DLOCKBUFF {
			Vector3D*		vtx;
			Color*			col;
			Vector2D*		tex;
			unsigned int	num;
		};
		// ���b�N�Ɏg�����̃o�b�t�@
		struct _3DLOCKBUFF {
			Vector3D*		vtx;
			Vector3D*		nor;
			Color*			col;
			Vector2D*		tex;
			unsigned int	num;
		};

		//==============================================================================
		// �|���S���R���e�i
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
			bool m_DethFlag;	// �폜�t���O

		};

		//==============================================================================
		// 3D�|���S��
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
		// 2D�|���S��
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
		// �S�Ẵ|���S���}�l�[�W��
		//------------------------------------------------------------------------------
		class PolygonManager
		{
		public:
			PolygonManager(DirectX *);
			virtual ~PolygonManager();

			// ������
			void Update(void);
			void Draw(void);

			// �쐬�Ƃ�
			Polygon2D* Create2D(const Vector2D& size = Vector2D(100.f, 100.f), const Color& col = Color(1, 1, 1, 1));
			
			// 3D�|���S���쐬
			template<class _T, class... Args> _T* Create3D(const Args&... args)
			{
				_T* buf = new _T(args...);

				buf->SetManager(this);

				if (m_3DusingNum + buf->GetVtxNum() > k3DMaxBuffer)
				{
					ASSERT(false, "3D�o�b�t�@�I�[�o�[");
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
			//�`��
			void Draw2DList(void);
			void Draw3DList(void);

			// ������
			void Init2DVtx(void);
			void Init3DVtx(void);

			// 2D�o�b�t�@���b�N
			void Lock2D(unsigned int vtxoffset, unsigned int vtxnum);
			void Unlock2D(void);
			// 3D�o�b�t�@���b�N
			void Lock3D(unsigned int vtxoffset, unsigned int vtxnum);
			void Unlock3D(void);

			//===================================
			// �|���S�����X�g
			std::vector<Polygon2D*> m_2DPolygonList;
			std::vector<Polygon3D*> m_3DPolygonList;

			//===================================
			// 2D�g�p
			// �`��p�o�b�t�@ 
			struct {
				IDirect3DVertexBuffer9* _vtx;
				IDirect3DVertexBuffer9* _col;
				IDirect3DVertexBuffer9* _tex;
			} m_2DVtxBuff;	

			// �g�p����o�b�t�@
			_2DLOCKBUFF m_2DLockBuff;				
			unsigned int m_2DusingNum;	// �g�p������
			LPD3DXEFFECT m_2DEffect;	// �V�F�[�_�p
			struct {
				D3DXHANDLE proj;
				D3DXHANDLE world;
				D3DXHANDLE tex;
			} m_2DHandle;				// �n���h��

			//===================================
			// 3D�g�p
			// �`��p�o�b�t�@ 
			struct {
				IDirect3DVertexBuffer9* _vtx;
				IDirect3DVertexBuffer9* _nor;
				IDirect3DVertexBuffer9* _col;
				IDirect3DVertexBuffer9* _tex;
			} m_3DVtxBuff;
			// �g�p����o�b�t�@
			_3DLOCKBUFF m_3DLockBuff;

			// �o�b�t�@
			IDirect3DVertexBuffer9* m_3DVtxBuff2;

			unsigned int m_3DusingNum;
			struct {
				D3DXHANDLE matWVP;
				D3DXHANDLE tex;
			} m_3DHandle;
			LPD3DXEFFECT m_3DEffect;	// �V�F�[�_�p

			// �J�����p
			D3DXVECTOR3 m_posCameraP;
			D3DXVECTOR3 m_posCameraR;
			D3DXVECTOR3 m_vecCameraU;

			// ���C�g�ʒu
			D3DXVECTOR3 m_LighPos;

			//===================================
			// ���_�錾
			LPDIRECT3DVERTEXDECLARATION9 m_p3DDec;
			LPDIRECT3DVERTEXDECLARATION9 m_p3DDec2;
			LPDIRECT3DVERTEXDECLARATION9 m_p2DDec;
			
			// �`��p�}�g���b�N�X
			D3DXMATRIX m_mtxView;
			D3DXMATRIX m_mtxProj;
			D3DXMATRIX m_mLightVP;

			// �e�p
			LPDIRECT3DSURFACE9		m_pShadowMapZ;		// �[�x�o�b�t�@
			LPDIRECT3DTEXTURE9		m_pShadowMap;		// �e�N�X�`��
			LPDIRECT3DSURFACE9		m_pShadowMapSurf;	// �T�[�t�F�X
			LPDIRECT3DTEXTURE9		m_pEdgeMap;			// �e�N�X�`��
			LPDIRECT3DSURFACE9		m_pEdgeMapSurf;		// �T�[�t�F�X
			LPDIRECT3DTEXTURE9		m_pSoftMap[2];		// �e�N�X�`��
			LPDIRECT3DSURFACE9		m_pSoftMapSurf[2];	// �T�[�t�F�X

			DirectX* m_pDirectX;
		};

		
	}
}

#endif
//EOF