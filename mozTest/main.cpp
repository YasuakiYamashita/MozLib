//==============================================================================
//
// �e�X�g�v���O����[main.cpp]
// Author : Yasuaki Yamashita : 2014/07/09
//
//------------------------------------------------------------------------------

//==============================================================================
// include
//------------------------------------------------------------------------------
#include <mozCommon.h>
#include <mozWindow.h>
#include <mozThread.h>
#include <mozMath.h>
#include <mozDirectX.h>

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

using namespace moz::math;

//==============================================================================
// DirectX
//------------------------------------------------------------------------------
class testDirectX : public moz::DirectX::DirectX
{
public:
	testDirectX(moz::window::window* window, bool bWindowMode = true) :moz::DirectX::DirectX(window, bWindowMode){};

protected:
	void Init(void)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		D3DCAPS9 Caps;
		pDevice->GetDeviceCaps(&Caps);
		int MaxStreamNum = Caps.MaxStreams;

		// 3D�p���_�t�H�[�}�b�g
		D3DVERTEXELEMENT9 Vtx3dDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 2, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		// 2D�p���_�t�H�[�}�b�g
		D3DVERTEXELEMENT9 Vtx2dDecl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vector3D)* 4,
			D3DUSAGE_WRITEONLY,
			NULL,
			D3DPOOL_MANAGED,
			&m_2Dvtx,
			NULL
			)))	{
			return ;
		}

		if (FAILED(pDevice->CreateVertexBuffer(sizeof(float)* 4 * 4,
			D3DUSAGE_WRITEONLY,
			NULL,
			D3DPOOL_MANAGED,
			&m_2Dcol,
			NULL
			)))	{
			return;
		}

		if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vector2D) * 4,
			D3DUSAGE_WRITEONLY,
			NULL,
			D3DPOOL_MANAGED,
			&m_2Dtex,
			NULL
			)))	{
			return;
		}


		Vector3D *vec;
		Color    *col;
		Vector2D *tex;

		m_2Dvtx->Lock(0, 0, (void **)&vec, 0);
		vec[0] = Vector3D(10.0f, 110.0f, 1.0f);
		vec[1] = Vector3D(10.0f, 10.0f, 1.0f);
		vec[2] = Vector3D(110.0f, 110.0f, 1.0f);
		vec[3] = Vector3D(110.0f, 10.0f, 1.0f);
		m_2Dvtx->Unlock();

		m_2Dcol->Lock(0, 0, (void **)&col, 0);
		col[0] = Color(1.f, 1.f, 1.f, 1.f);
		col[1] = Color(1.f, 1.f, 1.f, 1.f);
		col[2] = Color(1.f, 1.f, 1.f, 1.f);
		col[3] = Color(1.f, 1.f, 1.f, 1.f);
		m_2Dcol->Unlock();

		m_2Dtex->Lock(0, 0, (void **)&tex, 0);
		tex[0] = Vector2D(0.f, 1.f);
		tex[1] = Vector2D(0.f, 1.f);
		tex[2] = Vector2D(1.f, 0.f);
		tex[3] = Vector2D(0.f, 1.f);
		m_2Dtex->Unlock();
		pDevice->CreateVertexDeclaration(Vtx2dDecl, &m_p2DDec);

		// �G���[�擾
		LPD3DXBUFFER pError;

		// �V�F�[�_�R���p�C��
		if (FAILED(D3DXCreateEffectFromFile(pDevice, L"test.fx", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError)))
		{
			OutputDebugStringA((char *)pError->GetBufferPointer());
			pError->Release();
			return ;
		}
	}

	void Draw(void)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�f�[�^���M
		pDevice->SetVertexDeclaration(m_p2DDec);

		D3DXMATRIX proj(
			2 / (float)m_pWindow->GetWidth(), 0.0f, 0.0f, 0.0f,
			0.0f, -2 / (float)m_pWindow->GetHeight(), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
			);
		//pDevice->SetTransform(D3DTS_PROJECTION, &proj);


		//�l�X�ȃI�u�W�F�N�g�̕`�揈�����s��
		pDevice->SetStreamSource(0, m_2Dvtx, 0, sizeof(Vector3D));
		pDevice->SetStreamSource(1, m_2Dcol, 0, sizeof(Color));
		pDevice->SetStreamSource(2, m_2Dtex, 0, sizeof(Vector2D));

		m_pEffect->SetTechnique("basicTechnique");	// �e�N�j�b�N�ݒ�
		m_pEffect->Begin(nullptr, 0);
		m_pEffect->BeginPass(0u);
		m_pEffect->SetMatrix("proj", &proj);
		Color col(1, 1, 1, 1);
		m_pEffect->SetFloatArray("gColor", (float *)&col, 4);
		m_pEffect->CommitChanges();

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		m_pEffect->EndPass();
		m_pEffect->End();

	}

	void Update(void)
	{

	}

	void Uninit(void)
	{
		SAFE_RELEASE(m_p2DDec);
		SAFE_RELEASE(m_2Dvtx);
		SAFE_RELEASE(m_2Dcol);
		SAFE_RELEASE(m_2Dtex);
		SAFE_RELEASE(m_pEffect);
	}

	LPDIRECT3DVERTEXDECLARATION9 m_p2DDec;
	IDirect3DVertexBuffer9* m_2Dvtx;
	IDirect3DVertexBuffer9* m_2Dcol;
	IDirect3DVertexBuffer9* m_2Dtex;

	LPD3DXEFFECT m_pEffect;
};

//==============================================================================
// Window
//------------------------------------------------------------------------------
class testWindow : public moz::window::window, moz::thread::ThreadFunc
{
public:
	testWindow(HINSTANCE hInstance, const char * windowName) :
		window(hInstance, windowName){};

	moz::thread::Thread* _thread;

	void Init(void)
	{
		_thread = new moz::thread::Thread(this, false, this);
	}

	DWORD _Func(moz::thread::funcData* data)
	{
		moz::window::window * _wind = (moz::window::window*)data->data;
		m_directx = new testDirectX(_wind, true);

		// �J�n
		m_directx->run(&data->isExit);

		// �I��
		SAFE_DELETE(m_directx);

		return 0;
	}

	void Update()
	{
		if (_thread->IsDead())
		{
			Exit();
		}

		Sleep(1);
	}

	void Uninit(void)
	{
		_thread->JoinSleep();
		delete _thread;
	}


protected:
	void _wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			// �L�[���̓��b�Z�[�W
		case WM_KEYDOWN:

			// �p�����[�^����
			switch (wParam)
			{
				// �G�X�P�[�v�L�[
			case VK_ESCAPE:
#ifdef _DEBUG
				_thread->Exit();
#else
			{
					int nData = MessageBox(hWnd, TEXT("�I�����܂����H"), TEXT("�I���m�F"), MB_OKCANCEL | MB_ICONQUESTION);
					if (nData == IDOK)
					{
						// �E�B���h�E�폜
						Exit();
					}
				}
#endif
				break;
			case VK_RETURN:
				//_thread->Exit();
				//m_directx->FlipScreenMode();

				break;
			}
			break;
		case WM_SYSKEYDOWN:     // Alt + ����L�[�̏����Ɏg��
			switch (wParam)
			{
			case VK_RETURN:     // Alt + Enter�������Ɛ؂�ւ�
				FlipWindowMode();
				break;
			default:
				break;
			}
			break;
		}
	}

	testDirectX* m_directx;
};

//------------------------------------------------------------------------------
// winmain
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	testWindow window(hInstance, "test");
	return window.run();
}
//EOF
