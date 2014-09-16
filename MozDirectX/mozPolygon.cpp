//==============================================================================
//
// �|���S���Ǘ�[mozPolygon.cpp]
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
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		PolygonManager::PolygonManager(DirectX * dev)
			:m_pDirectX(dev)
		{
			LPDIRECT3DDEVICE9 pDevice =  m_pDirectX->GetDevice();
			/*
			// �o�b�t�@�m��
			if (FAILED(pDevice->CreateVertexBuffer(
				sizeof(Vector4D)* kMaxBufferBoard2D,		// �ő吔���m��
				(D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY),	// ���I�m�ہA�������݂̂�
				NULL,										// 2DFVF�f�[�^
				D3DPOOL_DEFAULT,							// �������������z�u
				&m_2Dvtx,									// �����o�b�t�@
				NULL
				)))
			{
				//�A�T�[�g�`�F�b�N
				ASSERT((m_pVtxBuffBoard != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}

			// �o�b�t�@�m��
			if (FAILED(pDevice->CreateVertexBuffer(
				sizeof(Vector3D)* kMaxBufferBoard3D,		// �ő吔���m��
				(D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY),	// ���I�m�ہA�������݂̂�
				NULL,										// 2DFVF�f�[�^
				D3DPOOL_DEFAULT,							// �������������z�u
				&m_3Dvtx,									// �����o�b�t�@
				NULL
				)))
			{
				//�A�T�[�g�`�F�b�N
				ASSERT((m_pVtxBuffBoard != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}

			// ���_�f�[�^����
			pDevice->CreateVertexDeclaration(Vtx3dDecl, &m_p3DDec);
			pDevice->CreateVertexDeclaration(Vtx2dDecl, &m_p2DDec);
			*/
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		PolygonManager::~PolygonManager()
		{

		}
	}
}



//EOF