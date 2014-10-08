//==============================================================================
//
// 3D�|���S���쐬[moz3DPolygon.cpp]
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
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		PolygonBillboard::PolygonBillboard(const Vector2D& size)
			: Polygon3D(size)
		{
			SetVtxNum(4);
		}

		//==============================================================================
		// �f�X�g���N�^
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
		void PolygonBillboard::DrawUpdate(void)
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

namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		PolygonIndex::PolygonIndex(int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY)
			: m_nNumBlockX(nNumBlockX)
			, m_nNumBlockY(nNumBlockY)
			, m_fSizeBlockX(fSizeBlockX)
			, m_fSizeBlockY(fSizeBlockY)
			, Polygon3D(Vector2D(nNumBlockX*fSizeBlockX, nNumBlockY*fSizeBlockY))
		{
			unsigned int nNumVertex = 0u;
			nNumVertex = (m_nNumBlockX + 1) * (m_nNumBlockY + 1);
			m_nNumVertexIndex = ((m_nNumBlockX * 2 + 2) * m_nNumBlockY) + (m_nNumBlockY * 2 - 2);
			m_nNumPolygon = m_nNumVertexIndex - 2;

			SetVtxNum(nNumVertex);
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		PolygonIndex::~PolygonIndex()
		{
			SAFE_RELEASE(m_indexBuff);
		}

		//==============================================================================
		// SetVtx
		//------------------------------------------------------------------------------
		void PolygonIndex::SetVtx(const _3DLOCKBUFF* buff)
		{
			_InitVertexXZ(buff);
			_InitIndex();
		}

		//==============================================================================
		// SetIndex
		//------------------------------------------------------------------------------
		void PolygonIndex::_InitIndex(void)
		{
			// �C���f�b�N�X�@���
			if (m_indexBuff == nullptr)
			{
				WORD* pIndex = new WORD[m_nNumVertexIndex];

				int nXA1 = (int)(m_nNumBlockX + 1);
				int nXA2 = (int)(m_nNumBlockX + 2);
				int nXA1M2 = nXA1 * 2;
				int nXA2M2 = nXA2 * 2;
				for (unsigned int nCntIndex = 0; nCntIndex < m_nNumVertexIndex; nCntIndex++)
				{
					pIndex[nCntIndex] = (WORD)(((nXA1)+(nCntIndex / 2))									// ���{�l
						- (nCntIndex / nXA2M2)										// �s������
						- ((nCntIndex % 2) * (nXA1))								// ���v�Z
						- ((nCntIndex % (nXA2M2) / (nXA1M2)) * (nXA2))				// �E�㏈��
						+ ((nCntIndex % (nXA2M2) / (nXA1M2 + 1)) * (nXA1M2 + 1))	// ��������
						);
				}

				// �C���f�b�N�X�o�b�t�@�쐬
				m_indexBuff = m_manager->CreateIndex(m_nNumVertexIndex, pIndex);

				SAFE_DELETE_ARRAY(pIndex);
			}
		}

		//==============================================================================
		// XY���ʂɓW�J
		void PolygonIndex::_InitVertexXY(const _3DLOCKBUFF* buff)
		{
			for (unsigned int nCntBlock = 0; nCntBlock < GetVtxNum(); nCntBlock++)
			{
				buff->vtx[nCntBlock] = Vector3D(
					m_fSizeBlockX *  (nCntBlock % (m_nNumBlockX + 1) - (m_nNumBlockX / 2.0f)),
					m_fSizeBlockY * -(nCntBlock / (m_nNumBlockX + 1) - (m_nNumBlockY / 2.0f)),
					0.0f);
				buff->nor[nCntBlock] = Vector3D(0.0f, 0.0f, -1.0f);
				buff->col[nCntBlock] = m_col;
				buff->tex[nCntBlock] = Vector2D((float)(nCntBlock % (m_nNumBlockX + 1)), (float)(nCntBlock / (m_nNumBlockX + 1)));
			}
		}

		//==============================================================================
		// XZ���ʂɓW�J
		void PolygonIndex::_InitVertexXZ(const _3DLOCKBUFF* buff)
		{
			for (unsigned int nCntBlock = 0; nCntBlock < GetVtxNum(); nCntBlock++)
			{
				buff->vtx[nCntBlock] = Vector3D(
					m_fSizeBlockX *  (nCntBlock % (m_nNumBlockX + 1) - (m_nNumBlockX / 2.0f)),
					0.0f,
					m_fSizeBlockY * -(nCntBlock / (m_nNumBlockX + 1) - (m_nNumBlockY / 2.0f)));
				buff->nor[nCntBlock] = Vector3D(0.0f, 1.0f, 0.0f);
				buff->col[nCntBlock] = m_col;
				buff->tex[nCntBlock] = Vector2D((float)(nCntBlock % (m_nNumBlockX + 1)), (float)(nCntBlock / (m_nNumBlockX + 1)));
			}
		}

		//==============================================================================
		// ZY���ʂɓW�J
		void PolygonIndex::_InitVertexZY(const _3DLOCKBUFF* buff)
		{
			for (unsigned int nCntBlock = 0; nCntBlock < GetVtxNum(); nCntBlock++)
			{
				buff->vtx[nCntBlock] = Vector3D(
					0.0f,
					m_fSizeBlockY * -(nCntBlock / (m_nNumBlockX + 1) - (m_nNumBlockY / 2.0f)),
					m_fSizeBlockX *  (nCntBlock % (m_nNumBlockX + 1) - (m_nNumBlockX / 2.0f)));
				buff->nor[nCntBlock] = Vector3D(1.0f, 0.0f, 0.0f);
				buff->col[nCntBlock] = m_col;
				buff->tex[nCntBlock] = Vector2D((float)(nCntBlock % (m_nNumBlockX + 1)), (float)(nCntBlock / (m_nNumBlockX + 1)));
			}
		}

		//==============================================================================
		// XY���ʂɋt�W�J
		void PolygonIndex::_InitVertexXYReverse(const _3DLOCKBUFF* buff)
		{
			for (unsigned int nCntBlock = 0; nCntBlock < GetVtxNum(); nCntBlock++)
			{
				buff->vtx[nCntBlock] = Vector3D(
					m_fSizeBlockX * -(nCntBlock % (m_nNumBlockX + 1) - (m_nNumBlockX / 2.0f)),
					m_fSizeBlockY * -(nCntBlock / (m_nNumBlockX + 1) - (m_nNumBlockY / 2.0f)),
					0.0f);
				buff->nor[nCntBlock] = Vector3D(0.0f, 0.0f, 1.0f);
				buff->col[nCntBlock] = m_col;
				buff->tex[nCntBlock] = Vector2D((float)(nCntBlock % (m_nNumBlockX + 1)), (float)(nCntBlock / (m_nNumBlockX + 1)));
			}
		}

		//==============================================================================
		// XZ���ʂɋt�W�J
		void PolygonIndex::_InitVertexXZReverse(const _3DLOCKBUFF* buff)
		{
			for (unsigned int nCntBlock = 0; nCntBlock < GetVtxNum(); nCntBlock++)
			{
				buff->vtx[nCntBlock] = Vector3D(
					m_fSizeBlockX * -(nCntBlock % (m_nNumBlockX + 1) - (m_nNumBlockX / 2.0f)),
					0.0f,
					m_fSizeBlockY * -(nCntBlock / (m_nNumBlockX + 1) - (m_nNumBlockY / 2.0f)));
				buff->nor[nCntBlock] = Vector3D(0.0f, -1.0f, 0.0f);
				buff->col[nCntBlock] = m_col;
				buff->tex[nCntBlock] = Vector2D((float)(nCntBlock % (m_nNumBlockX + 1)), (float)(nCntBlock / (m_nNumBlockX + 1)));
			}
		}

		//==============================================================================
		// ZY���ʂɋt�W�J
		void PolygonIndex::_InitVertexZYReverse(const _3DLOCKBUFF* buff)
		{
			for (unsigned int nCntBlock = 0; nCntBlock < GetVtxNum(); nCntBlock++)
			{
				buff->vtx[nCntBlock] = Vector3D(
					0.0f,
					m_fSizeBlockY * -(nCntBlock / (m_nNumBlockX + 1) - (m_nNumBlockY / 2.0f)),
					m_fSizeBlockX * -(nCntBlock % (m_nNumBlockX + 1) - (m_nNumBlockX / 2.0f)));
				buff->nor[nCntBlock] = Vector3D(-1.0f, 0.0f, 0.0f);
				buff->col[nCntBlock] = m_col;
				buff->tex[nCntBlock] = Vector2D((float)(nCntBlock % (m_nNumBlockX + 1)), (float)(nCntBlock / (m_nNumBlockX + 1)));
			}
		}

		//==============================================================================
		// Draw
		//------------------------------------------------------------------------------
		void PolygonIndex::Draw(const LPDIRECT3DDEVICE9& device, unsigned int startvtx, unsigned int vtxnum)
		{
			device->SetIndices(m_indexBuff);
			device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, startvtx, 0, vtxnum, 0, m_nNumPolygon);
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void PolygonIndex::Update(void)
		{

		}
	}
}

//EOF