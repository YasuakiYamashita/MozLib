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
			, m_3DusingNum(0u)
		{
			const LPDIRECT3DDEVICE9& pDevice =  m_pDirectX->GetDevice();

			// 2D���_�f�[�^������
			Init2DVtx();
			// 3D���_�f�[�^������
			Init3DVtx();

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

			m_3DHandle.matWVP = m_3DEffect->GetParameterByName(NULL, "gMatWVP");
			m_3DHandle.tex = m_3DEffect->GetParameterByName(NULL, "gDiffuseMap");

			// �v���W�F�N�V�����}�g���b�N�X
			D3DXMatrixIdentity(&m_mtxProj);
			D3DXMatrixPerspectiveFovLH(&m_mtxProj,		// �v���W�F�N�V�����}�g���b�N�X�̏�����
				D3DX_PI / 4.0f,				// ����p
				(float)m_pDirectX->GetWindow()->GetWidth() / (float)m_pDirectX->GetWindow()->GetHeight(),	// �A�X�y�N�g��
				10.0f,						// rear�l
				10000.0f);					// far�l

			// �r���[�}�g���b�N�X
			m_posCameraP = D3DXVECTOR3(0.0f, 200, -300.0f);
			m_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXMatrixIdentity(&m_mtxView);	// �r���[�}�g���b�N�X�̏�����
			D3DXMatrixLookAtLH(&m_mtxView, &m_posCameraP, &m_posCameraR, &m_vecCameraU);

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

			SAFE_RELEASE(m_p3DDec);
			SAFE_RELEASE(m_3DVtxBuff._vtx);
			SAFE_RELEASE(m_3DVtxBuff._nor);
			SAFE_RELEASE(m_3DVtxBuff._col);
			SAFE_RELEASE(m_3DVtxBuff._tex);
			SAFE_RELEASE(m_3DEffect);
			for (auto it : m_3DPolygonList)
			{
				SAFE_DELETE(it);
			}

		}

		//==============================================================================
		// 2D�|���S��������
		//------------------------------------------------------------------------------
		void PolygonManager::Init2DVtx(void)
		{
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();

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
		// 3D�|���S��������
		//------------------------------------------------------------------------------
		void PolygonManager::Init3DVtx(void)
		{
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vector3D)* 4 * k3DMaxBuffer,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff._vtx,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff._vtx != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vector3D)* 4 * k3DMaxBuffer,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff._nor,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff._vtx != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(Color)* 4 * k3DMaxBuffer,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff._col,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff._vtx != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}

			if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vector2D)* 4 * k3DMaxBuffer,
				D3DUSAGE_WRITEONLY,
				NULL,
				D3DPOOL_MANAGED,
				&m_3DVtxBuff._tex,
				NULL
				)))	{
				ASSERT((m_3DVtxBuff._vtx != NULL), "���_�o�b�t�@���m�ۂł��܂���ł����B");
				return;
			}

		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void PolygonManager::Update(void)
		{
			for (auto it : m_3DPolygonList)
			{
				it->Update();
			}

			for (auto it: m_2DPolygonList)
			{
				it->Update();
			}
		}
		
		//==============================================================================
		// �`��
		//------------------------------------------------------------------------------
		void PolygonManager::Draw(void)
		{
			Draw3DList();
			Draw2DList();
		}

		//==============================================================================
		// Draw3D
		//------------------------------------------------------------------------------
		void PolygonManager::Draw3DList(void)
		{
			// �f�o�C�X
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();
			int startvtx = 0;

			// ���_�f�[�^���M
			pDevice->SetVertexDeclaration(m_p3DDec);

			//�l�X�ȃI�u�W�F�N�g�̕`�揈�����s��
			pDevice->SetStreamSource(0, m_3DVtxBuff._vtx, 0, sizeof(Vector3D));
			pDevice->SetStreamSource(1, m_3DVtxBuff._nor, 0, sizeof(Vector3D));
			pDevice->SetStreamSource(2, m_3DVtxBuff._col, 0, sizeof(Color));
			pDevice->SetStreamSource(3, m_3DVtxBuff._tex, 0, sizeof(Vector2D));

			// �V�F�[�_�ݒ�
			m_3DEffect->Begin(nullptr, 0);

			// �}�g���b�N�X�ݒ�
			for (auto it : m_3DPolygonList)
			{
				D3DXMATRIX matWVP;

				// ���h���[���Ă�
				it->DrawUpdate();

				matWVP = it->GetWorldMtx() * m_mtxView * m_mtxProj;

				// �s��ݒ�
				m_3DEffect->SetMatrix(m_3DHandle.matWVP, &matWVP);	// �s��Z�b�g

				// �e�N�X�`���f�[�^�[
				if (it->GetTex())
					m_3DEffect->SetTexture(m_3DHandle.tex, it->GetTex()->GetTex());

				m_3DEffect->CommitChanges();

				// �e�N�X�`���L��ƂȂ��Ńp�X��ς���
				m_3DEffect->BeginPass(it->GetTex() ? 1u : 0u);

				// �h���[�I�I
				it->Draw(pDevice, startvtx, it->GetVtxNum());

				// �o��
				startvtx += it->GetVtxNum();

				// �I��
				m_3DEffect->EndPass();
			}

			// �V�F�[�_�I��
			m_3DEffect->End();
		}

		//==============================================================================
		// Draw2D
		//------------------------------------------------------------------------------
		void PolygonManager::Draw2DList(void)
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

				// �e�N�X�`���ݒ�
				if (it->GetTex())
					m_2DEffect->SetTexture(m_2DHandle.tex, it->GetTex()->GetTex());

				m_2DEffect->CommitChanges();

				// �h���[
				it->DrawUpdate();

				// �e�N�X�`���L��ƂȂ��Ńp�X��ς���
				m_2DEffect->BeginPass(it->GetTex() ? 1u : 0u);

				// �h���[
				it->Draw(pDevice, startvtx, it->GetVtxNum());

				startvtx += it->GetVtxNum();

				// �V�F�[�_�I��
				m_2DEffect->EndPass();

			}

			// �V�F�[�_�I��
			m_2DEffect->End();
		}

		//==============================================================================
		// 2D�|���S���쐬
		//------------------------------------------------------------------------------
		Polygon2D* PolygonManager::Create2D(const Vector2D& size, const Color& col)
		{
			Polygon2D* buf = new Polygon2D(size);

			if (m_2DusingNum + buf->GetVtxNum() > k2DMaxBuffer)
			{
				ASSERT(false, "2D�o�b�t�@�I�[�o�[");
				SAFE_DELETE(buf);
				return nullptr;
			}

			Lock2D(m_2DusingNum, buf->GetVtxNum());
			buf->SetVtx(&m_2DLockBuff);
			Unlock2D();

			m_2DusingNum += buf->GetVtxNum();
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

		//==============================================================================
		// 3D ���b�N
		//------------------------------------------------------------------------------
		void PolygonManager::Lock3D(unsigned int vtxoffset, unsigned int vtxnum)
		{
			// ���b�N����Ă��Ȃ����m�F
			if (m_3DLockBuff.col != nullptr &&
				m_3DLockBuff.tex != nullptr &&
				m_3DLockBuff.vtx != nullptr &&
				m_3DLockBuff.nor != nullptr)
			{
				// �����I�ɃA�����b�N
				Unlock3D();
			}

			const int size = sizeof(float);

			// �T�C�Y�����Ă���
			m_3DLockBuff.num = vtxnum;
			m_3DVtxBuff._vtx->Lock(size * 3 * vtxoffset, size * 3 * vtxnum, (void **)&m_3DLockBuff.vtx, 0);
			m_3DVtxBuff._col->Lock(size * 4 * vtxoffset, size * 4 * vtxnum, (void **)&m_3DLockBuff.col, 0);
			m_3DVtxBuff._tex->Lock(size * 2 * vtxoffset, size * 2 * vtxnum, (void **)&m_3DLockBuff.tex, 0);
			m_3DVtxBuff._nor->Lock(size * 2 * vtxoffset, size * 2 * vtxnum, (void **)&m_3DLockBuff.nor, 0);
		}

		//==============================================================================
		// �C���f�b�N�X
		//------------------------------------------------------------------------------
		LPDIRECT3DINDEXBUFFER9 PolygonManager::CreateIndex(unsigned int IndexNum,const WORD* Index)
		{
			LPDIRECT3DINDEXBUFFER9 indexBuff;
			const LPDIRECT3DDEVICE9& pDevice = m_pDirectX->GetDevice();
			WORD * pIndex = nullptr;
			
			// �C���f�b�N�X�o�b�t�@�擾
			if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD)* IndexNum,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&indexBuff,
				NULL)))
			{
				ASSERT(false, "�C���f�b�N�X�o�b�t�@���擾�o���܂���ł���");
				return nullptr;
			}

			// Index �i�[
			if (Index != nullptr)
			{
				indexBuff->Lock(0, 0, (void **)&pIndex, 0);
				for (unsigned int i = 0; i < IndexNum; ++i)
				{
					pIndex[i] = Index[i];
				}
				indexBuff->Unlock();
			}

			return indexBuff;
		}

		//==============================================================================
		// 3D �A�����b�N
		//------------------------------------------------------------------------------
		void PolygonManager::Unlock3D(void)
		{
			// �F
			if (m_3DLockBuff.col != nullptr)
			{
				m_3DVtxBuff._col->Unlock();
				m_3DLockBuff.col = nullptr;
			}
			// �e�N�X�`��
			if (m_3DLockBuff.tex != nullptr)
			{
				m_3DVtxBuff._tex->Unlock();
				m_3DLockBuff.tex = nullptr;
			}
			// ���_
			if (m_3DLockBuff.vtx != nullptr)
			{
				m_3DVtxBuff._vtx->Unlock();
				m_3DLockBuff.vtx = nullptr;
			}
			// �@��
			if (m_3DLockBuff.nor != nullptr)
			{
				m_3DVtxBuff._nor->Unlock();
				m_3DLockBuff.nor = nullptr;
			}
		}

		//==============================================================================
		// �|���S���R���e�i
		//------------------------------------------------------------------------------
		void PolygonContainer::Draw(const LPDIRECT3DDEVICE9& device, unsigned int startvtx, unsigned int vtxnum)
		{
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, startvtx, (vtxnum + 1) / 2);
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		PolygonContainer::~PolygonContainer()
		{
			SAFE_RELEASE(m_indexBuff);
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
		Polygon2D::Polygon2D(const Vector2D& size)
			: m_size(size)
		{
			SetVtxNum(4);
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		Polygon2D::~Polygon2D()
		{

		}

		//==============================================================================
		// SetVtx
		//------------------------------------------------------------------------------
		void Polygon2D::SetVtx(const _2DLOCKBUFF* buff)
		{
			buff->vtx[0] = Vector3D(-m_size.v.x / 2.f, m_size.v.y / 2.f, 1.0f);
			buff->vtx[1] = Vector3D(-m_size.v.x / 2.f, -m_size.v.y / 2.f, 1.0f);
			buff->vtx[2] = Vector3D(m_size.v.x / 2.f, m_size.v.y / 2.f, 1.0f);
			buff->vtx[3] = Vector3D(m_size.v.x / 2.f, -m_size.v.y / 2.f, 1.0f);

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
		void Polygon2D::DrawUpdate(void)
		{
		
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void Polygon2D::Update(void)
		{

		}
	}
}

//==============================================================================
// 3D�}�l�[�W���[
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		Polygon3D::Polygon3D(const Vector2D& size)
			: m_size(size)
		{
			SetVtxNum(4);
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		Polygon3D::~Polygon3D()
		{

		}

		//==============================================================================
		// SetVtx
		//------------------------------------------------------------------------------
		void Polygon3D::SetVtx(const _3DLOCKBUFF* buff)
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
		void Polygon3D::DrawUpdate(void)
		{
			D3DXMATRIX mtxRot;
			D3DXMatrixIdentity(&mtxRot);
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.v.y, m_rot.v.x, m_rot.v.z);

			m_mtxWorld._11 = m_scl.v.x * mtxRot._11;
			m_mtxWorld._12 = m_scl.v.x * mtxRot._12;
			m_mtxWorld._13 = m_scl.v.x * mtxRot._13;
			m_mtxWorld._14 = 0.0f;

			m_mtxWorld._21 = m_scl.v.y * mtxRot._21;
			m_mtxWorld._22 = m_scl.v.y * mtxRot._22;
			m_mtxWorld._23 = m_scl.v.y * mtxRot._23;
			m_mtxWorld._24 = 0.0f;

			m_mtxWorld._31 = m_scl.v.z * mtxRot._31;
			m_mtxWorld._32 = m_scl.v.z * mtxRot._32;
			m_mtxWorld._33 = m_scl.v.z * mtxRot._33;
			m_mtxWorld._34 = 0.0f;

			m_mtxWorld._41 = m_pos.v.x;
			m_mtxWorld._42 = m_pos.v.y;
			m_mtxWorld._43 = m_pos.v.z;
			m_mtxWorld._44 = 1.0f;
		}

		//==============================================================================
		// Update
		//------------------------------------------------------------------------------
		void Polygon3D::Update(void)
		{

		}
	}
}

//EOF