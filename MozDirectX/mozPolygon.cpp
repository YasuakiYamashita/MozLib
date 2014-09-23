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

//==============================================================================
// Polygon Manager
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		PolygonManager::PolygonManager(DirectX * dev)
			: m_pDirectX(dev)
			, m_2DusingNum(0u)
			, m_2DLockBuff({ nullptr, nullptr, nullptr })
			, m_2DVtxBuff({ nullptr, nullptr, nullptr })
		{
			LPDIRECT3DDEVICE9 pDevice =  m_pDirectX->GetDevice();

			// 2D���_�f�[�^������
			Init2DVtx();

			// ���_�f�[�^����
			pDevice->CreateVertexDeclaration(Vtx3dDecl, &m_p3DDec);
			pDevice->CreateVertexDeclaration(Vtx2dDecl, &m_p2DDec);

			// �G���[�擾
			LPD3DXBUFFER pError;

			// �V�F�[�_�R���p�C��
			if (FAILED(D3DXCreateEffectFromFile(pDevice, "./data/fx/2d.fx", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_2DEffect, &pError)))
			{
				OutputDebugStringA((char *)pError->GetBufferPointer());
				ASSERT(false, "%s", (char *)pError->GetBufferPointer());
				pError->Release();
				return;
			}

			// �V�F�[�_�R���p�C��
			if (FAILED(D3DXCreateEffectFromFile(pDevice, "./data/fx/3d.fx", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_3DEffect, &pError)))
			{
				OutputDebugStringA((char *)pError->GetBufferPointer());
				ASSERT(false, "%s", (char *)pError->GetBufferPointer());
				pError->Release();
				return;
			}

			// �e�N�j�b�N�ݒ�
			m_2DEffect->SetTechnique("basicTechnique");
			m_3DEffect->SetTechnique("basicTechnique");

			// �n���h���擾
			m_2DHandle.proj = m_2DEffect->GetParameterByName(NULL, "mtxProj");
			m_2DHandle.world = m_2DEffect->GetParameterByName(NULL, "mtxWorld");
			m_2DHandle.tex = m_2DEffect->GetParameterByName(NULL, "gDiffuseMap");
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		PolygonManager::~PolygonManager()
		{
			SAFE_RELEASE(m_p2DDec);
			SAFE_RELEASE(m_2DVtxBuff._vtx);
			SAFE_RELEASE(m_2DVtxBuff._col);
			SAFE_RELEASE(m_2DVtxBuff._tex);
			SAFE_RELEASE(m_2DEffect);

			for (auto it: m_2DPolygonList)
			{
				SAFE_DELETE(it);
			}
		}

		//==============================================================================
		// 2D�|���S��������
		//------------------------------------------------------------------------------
		void PolygonManager::Init2DVtx(void)
		{
			LPDIRECT3DDEVICE9 pDevice = m_pDirectX->GetDevice();

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(float)* 3 * k2DMaxBuffer,
				(D3DUSAGE_WRITEONLY),
				NULL,
				D3DPOOL_MANAGED,
				&m_2DVtxBuff._vtx,
				NULL
				)))	{
				ASSERT((m_2DVtxBuff._vtx != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(float)* 4 * k2DMaxBuffer,
				(D3DUSAGE_WRITEONLY),
				NULL,
				D3DPOOL_MANAGED,
				&m_2DVtxBuff._col,
				NULL
				)))	{
				ASSERT((m_2DVtxBuff._col != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(float)* 2 * k2DMaxBuffer,
				(D3DUSAGE_WRITEONLY),
				NULL,
				D3DPOOL_MANAGED,
				&m_2DVtxBuff._tex,
				NULL
				)))	{
				ASSERT((m_2DVtxBuff._tex != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void PolygonManager::Update(void)
		{
			for (auto it: m_2DPolygonList)
			{
				it->Update();
			}
		}

		//==============================================================================
		// Draw
		//------------------------------------------------------------------------------
		void PolygonManager::Draw(void)
		{
			// �f�o�C�X
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();
			D3DXMATRIX mtxWorld, mtxTmp;
			int startvtx = 0;

			// 
			const D3DXMATRIX mtxProj(
				2 / (float)m_pDirectX->GetWindow()->GetWidth(), 0.0f, 0.0f, 0.0f,
				0.0f, -2 / (float)m_pDirectX->GetWindow()->GetHeight(), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f, 1.0f
				);

			// ���_
			pDevice->SetVertexDeclaration(m_p2DDec);

			// 2D�`��
			pDevice->SetStreamSource(0, m_2DVtxBuff._vtx, 0, sizeof(Vector3D));
			pDevice->SetStreamSource(1, m_2DVtxBuff._col, 0, sizeof(Color));
			pDevice->SetStreamSource(2, m_2DVtxBuff._tex, 0, sizeof(Vector2D));

			// �V�F�[�_�J�n
			m_2DEffect->Begin(nullptr, 0);
			m_2DEffect->BeginPass(1u);

			// ����`��
			for (auto it: m_2DPolygonList)
			{
				const float sinb = sinf(it->GetRot().v.y);
				const float cosb = cosf(it->GetRot().v.y);

				D3DXMatrixIdentity(&mtxWorld);

				// �ړ��s��
				D3DXMatrixIdentity(&mtxTmp);
				mtxTmp._41 = it->GetPos().v.x;
				mtxTmp._42 = it->GetPos().v.y;
				mtxWorld = mtxTmp * mtxWorld;

				// ��]�s��
				D3DXMatrixIdentity(&mtxTmp);
				mtxTmp._11 = cosb;
				mtxTmp._12 = sinb;
				mtxTmp._21 = -sinb;
				mtxTmp._22 = cosb;
				mtxWorld = mtxTmp * mtxWorld;

				// �V�F�[�_�ݒ�
				m_2DEffect->SetMatrix(m_2DHandle.proj, &mtxProj);
				m_2DEffect->SetMatrix(m_2DHandle.world, &mtxWorld);
				m_2DEffect->SetTexture(m_2DHandle.tex, it->GetTex()->GetTex());
				m_2DEffect->CommitChanges();

				// �h���[
				it->Draw();
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, startvtx, (it->GetVtxNum() + 1) / 2);
				startvtx += it->GetVtxNum();
			}

			// �V�F�[�_�I��
			m_2DEffect->EndPass();
			m_2DEffect->End();
		}

		//==============================================================================
		// 2D�|���S���쐬
		//------------------------------------------------------------------------------
		Draw2D* PolygonManager::Create2D(const Vector2D& size, const Color& col)
		{
			if (m_2DusingNum + 4 > k2DMaxBuffer)
			{
				ASSERT(false, "2D�o�b�t�@�I�[�o�[");
				return nullptr;
			}

			Draw2D* buf = new Draw2D(size);

			Lock2D(m_2DusingNum, 4);
			m_2DLockBuff.vtx[0] = Vector3D(-size.v.x / 2.f, size.v.y / 2.f, 1.0f);
			m_2DLockBuff.vtx[1] = Vector3D(-size.v.x / 2.f, -size.v.y / 2.f, 1.0f);
			m_2DLockBuff.vtx[2] = Vector3D(size.v.x / 2.f, size.v.y / 2.f, 1.0f);
			m_2DLockBuff.vtx[3] = Vector3D(size.v.x / 2.f, -size.v.y / 2.f, 1.0f);

			m_2DLockBuff.col[0] = col;
			m_2DLockBuff.col[1] = col;
			m_2DLockBuff.col[2] = col;
			m_2DLockBuff.col[3] = col;

			m_2DLockBuff.tex[0] = Vector2D(0, 1);
			m_2DLockBuff.tex[1] = Vector2D(0, 0);
			m_2DLockBuff.tex[2] = Vector2D(1, 1);
			m_2DLockBuff.tex[3] = Vector2D(1, 0);
			Unlock2D();

			m_2DusingNum += 4;
			m_2DPolygonList.push_back(buf);

			return buf;
		}

		//==============================================================================
		// 2D ���b�N
		//------------------------------------------------------------------------------
		void PolygonManager::Lock2D(unsigned int vtxoffset, unsigned int vtxnum)
		{
			// ���b�N����Ă��Ȃ����m�F
			if (m_2DLockBuff.col != nullptr &&
				m_2DLockBuff.tex != nullptr &&
				m_2DLockBuff.vtx != nullptr)
			{
				// �����I�ɃA�����b�N
				Unlock2D();
			}

			const int size = sizeof(float);

			// �T�C�Y�����Ă���
			m_2DLockBuff.num = vtxnum;
			m_2DVtxBuff._vtx->Lock(size * 3 * vtxoffset, size * 3 * vtxnum, (void **)&m_2DLockBuff.vtx, 0);
			m_2DVtxBuff._col->Lock(size * 4 * vtxoffset, size * 4 * vtxnum, (void **)&m_2DLockBuff.col, 0);
			m_2DVtxBuff._tex->Lock(size * 2 * vtxoffset, size * 2 * vtxnum, (void **)&m_2DLockBuff.tex, 0);
		}

		//==============================================================================
		// 2D �A�����b�N
		//------------------------------------------------------------------------------
		void PolygonManager::Unlock2D(void)
		{
			// �F
			if (m_2DLockBuff.col != nullptr)
			{
				m_2DVtxBuff._col->Unlock();
				m_2DLockBuff.col = nullptr;
			}
			// �e�N�X�`��
			if (m_2DLockBuff.tex != nullptr)
			{
				m_2DVtxBuff._tex->Unlock();
				m_2DLockBuff.tex = nullptr;
			}
			// ���_
			if (m_2DLockBuff.vtx != nullptr)
			{
				m_2DVtxBuff._vtx->Unlock();
				m_2DLockBuff.vtx = nullptr;
			}
		}
	}
}

//==============================================================================
// 2D�}�l�[�W���[
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		Draw2D::Draw2D(const Vector2D& size)
			: m_size(size)
		{
			SetVtxNum(4);
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		Draw2D::~Draw2D()
		{

		}

		//==============================================================================
		// Draw
		//------------------------------------------------------------------------------
		void Draw2D::Draw(void)
		{
		
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void Draw2D::Update(void)
		{

		}
	}
}

//EOF