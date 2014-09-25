//==============================================================================
//
// 3D�|���S���쐬[moz3DPolygon.h]
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
		// �r���{�[�h�p
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
		// Index�L��̏ꍇ
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
			// �e�ʂ̒��_�ݒ�
			void _InitVertexXY(const _3DLOCKBUFF* buff);
			void _InitVertexXZ(const _3DLOCKBUFF* buff);
			void _InitVertexZY(const _3DLOCKBUFF* buff);
			void _InitVertexXYReverse(const _3DLOCKBUFF* buff);
			void _InitVertexXZReverse(const _3DLOCKBUFF* buff);
			void _InitVertexZYReverse(const _3DLOCKBUFF* buff);


		protected:
			// �u���b�N�X�̐�
			unsigned int m_nNumBlockX, m_nNumBlockY;

			// �u���b�N������̑傫��
			float m_fSizeBlockX, m_fSizeBlockY;

			// �K�v��
			unsigned int m_nNumVertexIndex, m_nNumPolygon;

		};

	}
}
#endif
//EOF