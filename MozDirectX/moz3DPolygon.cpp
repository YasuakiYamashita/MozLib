//==============================================================================
//
// 3Dポリゴン作成[moz3DPolygon.cpp]
// Author : Yasuaki Yamashita : 2014/09/24
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "moz3DPolygon.h"

namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		PolygonBillboard::PolygonBillboard(const Vector2D& size)
			: Polygon3D(size)
		{
			SetVtxNum(4);
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		PolygonBillboard::~PolygonBillboard()
		{

		}

		//==============================================================================
		// SetVtx
		//------------------------------------------------------------------------------
		void PolygonBillboard::SetVtx(const _3DLOCKBUFF* buff)
		{
			buff->vtx[0] = Vector3D(-m_size.v.x / 2.f, -m_size.v.y / 2.f, 0.0f);
			buff->vtx[1] = Vector3D(-m_size.v.x / 2.f, m_size.v.y / 2.f, 0.0f);
			buff->vtx[2] = Vector3D(m_size.v.x / 2.f, -m_size.v.y / 2.f, 0.0f);
			buff->vtx[3] = Vector3D(m_size.v.x / 2.f, m_size.v.y / 2.f, 0.0f);

			buff->nor[0] = Vector3D(0, 1, 0);
			buff->nor[1] = Vector3D(0, 1, 0);
			buff->nor[2] = Vector3D(0, 1, 0);
			buff->nor[3] = Vector3D(0, 1, 0);

			buff->col[0] = m_col;
			buff->col[1] = m_col;
			buff->col[2] = m_col;
			buff->col[3] = m_col;

			buff->tex[0] = Vector2D(0, 1);
			buff->tex[1] = Vector2D(0, 0);
			buff->tex[2] = Vector2D(1, 1);
			buff->tex[3] = Vector2D(1, 0);
		}

		//==============================================================================
		// Draw
		//------------------------------------------------------------------------------
		void PolygonBillboard::Draw(void)
		{
			D3DXMatrixInverse(&m_mtxWorld, NULL, &m_manager->GetViewMtx());
			m_mtxWorld._41 = m_pos.v.x;
			m_mtxWorld._42 = m_pos.v.y;
			m_mtxWorld._43 = m_pos.v.z;
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void PolygonBillboard::Update(void)
		{

		}
	}
}

//EOF