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

//==============================================================================
// �|���S���\���p
//------------------------------------------------------------------------------
namespace moz
{
	namespace DirectX
	{
		// 3D�p���_�t�H�[�}�b�g
		static const D3DVERTEXELEMENT9 3dDecl = 
		{
			{ 0, 0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,	0 },
			{ 1, 0, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_NORMAL,	0 },
			{ 2, 0,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,		0 },
			{ 3, 0, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	0 },
			D3DDECL_END()
		};

		// 2D�p���_�t�H�[�}�b�g
		static const D3DVERTEXELEMENT9 2dDecl =
		{
			{ 1, 0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITIONT,	0 },
			{ 2, 0,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,		0 },
			{ 3, 0,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	0 },
			D3DDECL_END()
		};

		// �ÓI�m��
		static const k3DPolygonNum = 10000;	// 3D�|���S��
		static const k2DPolygonNum = 100;	// 2D�|���S��

		//==============================================================================
		// �|���S���R���e�i
		//------------------------------------------------------------------------------
		class DrawList
		{
		public:
			DrawList(){};
			virtual ~DrawList();
			
			virtual void Draw() = 0;
			virtual void Update() = 0;

		private:
			IDirect3DVertexBuffer9* m_vtxDecl;
		};

		//==============================================================================
		// 3D�|���S��
		//------------------------------------------------------------------------------
		class DrawList3D
		{
		public:
			DrawList3D();
			virtual ~DrawList3D();

			virtual void Draw(void);
			virtual void Update(void);


		};

		//==============================================================================
		// �S�Ẵ|���S���}�l�[�W��
		//------------------------------------------------------------------------------
		class PolygonManager
		{
		public:
			PolygonManager();
			virtual ~PolygonManager();

			// ������
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



		};

		
	}
}

#endif
//EOF