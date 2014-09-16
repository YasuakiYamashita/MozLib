//==============================================================================
//
// ポリゴン管理[mozPolygon.cpp]
// Author : Yasuaki Yamashita : 2014/08/22
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "mozPolygon.h"

namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		PolygonManager::PolygonManager(DirectX * dev)
			:m_pDirectX(dev)
		{
			LPDIRECT3DDEVICE9 pDevice =  m_pDirectX->GetDevice();
			/*
			// バッファ確保
			if (FAILED(pDevice->CreateVertexBuffer(
				sizeof(Vector4D)* kMaxBufferBoard2D,		// 最大数分確保
				(D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY),	// 動的確保、書き込みのみ
				NULL,										// 2DFVFデータ
				D3DPOOL_DEFAULT,							// メモリを自動配置
				&m_2Dvtx,									// 統括バッファ
				NULL
				)))
			{
				//アサートチェック
				ASSERT((m_pVtxBuffBoard != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

			// バッファ確保
			if (FAILED(pDevice->CreateVertexBuffer(
				sizeof(Vector3D)* kMaxBufferBoard3D,		// 最大数分確保
				(D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY),	// 動的確保、書き込みのみ
				NULL,										// 2DFVFデータ
				D3DPOOL_DEFAULT,							// メモリを自動配置
				&m_3Dvtx,									// 統括バッファ
				NULL
				)))
			{
				//アサートチェック
				ASSERT((m_pVtxBuffBoard != NULL), "頂点バッファが確保できませんでした。");
				return;
			}

			// 頂点データ生成
			pDevice->CreateVertexDeclaration(Vtx3dDecl, &m_p3DDec);
			pDevice->CreateVertexDeclaration(Vtx2dDecl, &m_p2DDec);
			*/
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		PolygonManager::~PolygonManager()
		{

		}
	}
}



//EOF