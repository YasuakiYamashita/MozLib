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


using namespace moz::math;

//==============================================================================
// �|���S���\���p
//------------------------------------------------------------------------------
namespace moz
{

	namespace DirectX
	{
		// 3D�p���_�t�H�[�}�b�g
		static const D3DVERTEXELEMENT9 Vtx3dDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 2, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
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
		static const int k3DMaxBuffer = 400;	// 3D�|���S��
		static const int k2DMaxBuffer = 4000;	// 2D�|���S��

		//==============================================================================
		// �|���S���R���e�i
		//------------------------------------------------------------------------------
		class DrawList
		{
		public:
			DrawList()
				: m_DethFlag(false)
				, m_rot(0, 0, 0)
				, m_pos(0, 0, 0)
				, m_scl(0, 0, 0)
				, m_tex(nullptr)
				, m_vtxNum(0)
			{};
			virtual ~DrawList(){};
			
			virtual void Draw() = 0;
			virtual void Update() = 0;

			Vector3D& GetRot(void) { return m_rot; }
			Vector3D& GetPos(void) { return m_pos; }
			Vector3D& GetScl(void) { return m_scl; }

			TexContainer*& GetTex(void) { return m_tex; }
			
			unsigned int GetVtxNum(void) { return m_vtxNum; }
			bool GetDethFlag(void){ return m_DethFlag; }


		protected:
			Vector3D m_rot;
			Vector3D m_pos;
			Vector3D m_scl;


			void SetVtxNum(unsigned int num) { m_vtxNum = num; }

		private:
			unsigned int m_vtxNum;
			TexContainer *m_tex;
			bool m_DethFlag;	// �폜�t���O
		};

		//==============================================================================
		// 3D�|���S��
		//------------------------------------------------------------------------------
		class Draw3D : public DrawList
		{
		public:
			Draw3D();
			virtual ~Draw3D();

			virtual void Draw(void);
			virtual void Update(void);

		protected:

		};

		//==============================================================================
		// 2D�|���S��
		//------------------------------------------------------------------------------
		class Draw2D : public DrawList
		{
		public:
			Draw2D(const Vector2D& size);
			virtual ~Draw2D();

			virtual void Draw(void);
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
			Draw2D* Create2D(const Vector2D& size = Vector2D(100.f, 100.f), const Color& col = Color(1, 1, 1, 1));

		private:
			// ������
			void Init2DVtx(void);

			// 2D�o�b�t�@���b�N
			void Lock2D(unsigned int vtxoffset, unsigned int vtxnum);
			void Unlock2D(void);

			//===================================
			// �|���S�����X�g
			std::vector<Draw2D*> m_2DPolygonList;
			std::vector<Draw3D*> m_3DPolygonList;
			//std::list<DrawList*> m_EffectPolygonList;

			//===================================
			// 2D�g�p
			struct {
				IDirect3DVertexBuffer9* _vtx;
				IDirect3DVertexBuffer9* _col;
				IDirect3DVertexBuffer9* _tex;
			} m_2DVtxBuff;				// �`��p�o�b�t�@ 
			struct {
				Vector3D*		vtx;
				Color*			col;
				Vector2D*		tex;
				unsigned int	num;
			} m_2DLockBuff;				// �g�p����o�b�t�@
			unsigned int m_2DusingNum;	// �g�p������
			LPD3DXEFFECT m_2DEffect;	// �V�F�[�_�p
			struct {
				D3DXHANDLE proj;
				D3DXHANDLE pos;
				D3DXHANDLE rot;
			} m_2DHandle;				// �n���h��

			//===================================
			// 3D�g�p
			IDirect3DVertexBuffer9* m_3Dvtx;
			IDirect3DVertexBuffer9* m_Effectvtx;

			//===================================
			// ���_�錾
			LPDIRECT3DVERTEXDECLARATION9 m_p3DDec;
			LPDIRECT3DVERTEXDECLARATION9 m_p2DDec;

			DirectX* m_pDirectX;
		};

		
	}
}

#endif
//EOF