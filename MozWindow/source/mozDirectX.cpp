//==============================================================================
//
// DirectX�̃f�o�C�X[mozDirectX.cpp]
// Author : Yasuaki Yamashita : 2014/07/10
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include <mozCommon.h>
#include "mozWindow.h"

//------------------------------------------------------------------------------
// �f�o�C�X�擾
//------------------------------------------------------------------------------
namespace moz
{
	namespace window
	{
		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		DirectX::DirectX(window* _window, bool bWindowMode)
			: m_bWindowMode(bWindowMode)
			, m_pWindow(_window)
			, m_IsDeviceLost(false)
		{
			// �f�o�C�X�p�����[�^�p�̃��[�J���ϐ�
			D3DDISPLAYMODE d3ddm;


			// Direct3D�I�u�W�F�N�g�̍쐬
			LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
			if (pD3D == NULL)
			{
				// �쐬���s
				MessageBox(m_pWindow->GetHWnd(), TEXT("Direct3D�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���"), TEXT("�x��"), MB_OK);
				m_pWindow->Exit();
				return;
			}

			// ���݂̃f�B�X�v���C���[�h���擾
			if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
			{
				// �擾���s
				MessageBox(m_pWindow->GetHWnd(), TEXT("���݂̃f�B�X�v���C���[�h�̎擾�Ɏ��s���܂���"), TEXT("�x��"), MB_OK);
				m_pWindow->Exit();
				return;
			}

			// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
			ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));						// ���[�N���[���N���A
			m_d3dpp.BackBufferWidth			= m_pWindow->GetWidth();	// �Q�[����ʃT�C�Y(��)
			m_d3dpp.BackBufferHeight		= m_pWindow->GetHeight();	// �Q�[����ʃT�C�Y(����)
			m_d3dpp.BackBufferFormat		= d3ddm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
			m_d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
			m_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
			m_d3dpp.Windowed				= bWindowMode;				// �E�B���h�E���[�h
			m_d3dpp.hDeviceWindow			= m_pWindow->GetHWnd();		// �E�C���h�E�f�o�C�X
			m_d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
			m_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��
			m_d3dpp.Flags					= 0;						// �t���O
			m_d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;		// �}���`�T���v��
			m_d3dpp.MultiSampleQuality		= 0;						// �}���`�T���v��

			if (m_bWindowMode)
			{// �E�B���h�E���[�h
				m_d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
				m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;			// �C���^�[�o��
			}
			else
			{// �t���X�N���[�����[�h
				m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
				m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// �C���^�[�o��
			}

			// �f�o�C�X�I�u�W�F�N�g�̐���
			// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				m_pWindow->GetHWnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&m_d3dpp, &m_pDevice)))
			{
				// �擾���s
				MessageBox(m_pWindow->GetHWnd(), TEXT("�f�o�C�X�I�u�W�F�N�g�̐����Ɏ��s���܂���\r\n��������Â��\��������܂�"), TEXT("�x��"), MB_OK);
				m_pWindow->Exit();
				SAFE_RELEASE(pD3D);
				return;
			}

			// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
			m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// ���u�����h�̎��
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

			// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
			m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k���t�B���^���[�h��ݒ�
			m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g��t�B���^���[�h��ݒ�

			// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

			// �J��
			SAFE_RELEASE(pD3D);
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		DirectX::~DirectX()
		{
			// �f���[�g
			SAFE_RELEASE(m_pDevice);
		}


		//==============================================================================
		// ����
		//------------------------------------------------------------------------------
		void DirectX::run(bool* isExit)
		{
			const int	kGameFPS = 60;				// �b�ԃt���[����
			const int	kFPSMeasuringSpan = 500;				// FPS�v���Ԋu(�~���b)
			const int	kInMSecOneSec = 1000;				// 1�b���~���b�ŕ\��
			const float kInMSecOneSecFloat = 1000.0f;			// ��L������

			// FPS�����p�ϐ�
			DWORD dwExecLastTime;	// �Ō�ɏ�����������
			DWORD dwFPSLastTime;	// �Ō��FPS�v����������
			DWORD dwCurrentTime;	// ���ݎ���
			DWORD dwFrameCount;		// ���݃t���[����
			int fps = 0;			// ���݂�FPS

			dwExecLastTime = dwFPSLastTime = timeGetTime();	// ���ݎ������ŏI�������Ԃɐݒ�
			dwCurrentTime = dwFrameCount = 0;				// ������

			// ����������
			Init();

			while (!(*isExit))
			{
				// ���ݎ��Ԃ��擾
				dwCurrentTime = timeGetTime();

				// �f�o�C�X��D���Ԃ�
				DeviceTryBack();

				if (!m_IsDeviceLost)
				{
					// 0.5�b���Ƃ�FPS���v��
					if ((dwCurrentTime - dwFPSLastTime) >= kFPSMeasuringSpan)
					{
						fps = dwFrameCount * kInMSecOneSec / (dwCurrentTime - dwFPSLastTime);

						dwFPSLastTime = dwCurrentTime;	// FPS�v���������Ԃ��L�^
						dwFrameCount = 0;				// �b�ԃt���[���������Z�b�g

						// �E�B���h�E����ݒ�
						char cText[256];
						sprintf_s(cText, "%dfps:", fps);
						m_pWindow->SetWindowName(cText);

					}

					// FPS����(�O��̏������ԂƂ̍���(1/FPS)�b�ȏ�̏ꍇ�͏���)
					if ((dwCurrentTime - dwExecLastTime + (kGameFPS - fps)) >= (kInMSecOneSecFloat / kGameFPS))
					{
						// �����J�n���Ԃ��L�^(�����������ɑΉ����邽�߁A�I���ł͂Ȃ��J�n���Ԃ��L�^)
						dwExecLastTime = dwCurrentTime;

						// �V�[���̃N���A
						m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

						// �V�[���̕`��J�n
						if (SUCCEEDED(m_pDevice->BeginScene()))
						{
							// �A�b�v�f�[�g
							Update();

							// �`�揈��
							Draw();

							// �V�[���̕`��I��
							m_pDevice->EndScene();
						}

						// �V�[���̕\��
						if (m_pDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
						{
							m_IsDeviceLost = true;
						}

						// �b�ԃt���[�������L�^
						dwFrameCount++;
					}

				}

				// ���������߂���ꍇ�͂��傢���ƃX���[�v
				if (dwCurrentTime == timeGetTime())
				{
					Sleep(1);
				}
			}

			// �I������
			Uninit();

		}

		//==============================================================================
		// �S��ʂƃE�C���h�E��؂�ւ���
		//------------------------------------------------------------------------------
		void DirectX::FlipScreenMode(void)
		{
			m_bWindowMode = !m_bWindowMode;

			RECT rect;
			GetWindowRect(m_pWindow->GetHWnd(), &rect);

			if (m_bWindowMode)
			{// �E�B���h�E���[�h
				m_d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
				m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// �C���^�[�o��
				m_d3dpp.Windowed = TRUE;
			}
			else
			{// �t���X�N���[�����[�h
				m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
				m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// �C���^�[�o��
				m_d3dpp.Windowed = FALSE;
			}


			D3DPRESENT_PARAMETERS de = m_d3dpp;
			// ���傢�X���[�v
			Sleep(5);

			HRESULT hr = m_pDevice->Reset(&de);

			if (hr == D3DERR_DEVICELOST)
			{
				m_IsDeviceLost = true;
			}

			if (m_bWindowMode)
			{
				SetWindowLong(m_pWindow->GetHWnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX));
				SetWindowPos(m_pWindow->GetHWnd(),
					HWND_NOTOPMOST,                           //�t���X�N���[���͍őO�ʕ\������Ă���̂ŁA��������B
					rect.left,
					rect.top,
					rect.right - rect.left,
					rect.bottom - rect.top,
					SWP_SHOWWINDOW);
			}
			else
			{
				SetWindowLong(m_pWindow->GetHWnd(), GWL_STYLE, WS_POPUP | WS_VISIBLE);
				SetWindowPos(m_pWindow->GetHWnd(),
					HWND_TOPMOST,
					0,
					0,
					rect.right - rect.left,
					rect.bottom - rect.top,
					SWP_SHOWWINDOW);

			}

		}

		//==============================================================================
		// �f�o�C�X�擾����
		//------------------------------------------------------------------------------
		void DirectX::DeviceTryBack(void)
		{
			// �f�o�C�X�����X�g�����ꍇ
			if (m_IsDeviceLost && m_pWindow->GetActive())
			{
				Sleep(100); // 0.1�b�҂�

				// �f�o�C�X��Ԃ̃`�F�b�N
				HRESULT hr = m_pDevice->TestCooperativeLevel();
				if (FAILED(hr))
				{
					if (hr == D3DERR_DEVICELOST)
						return ;  // �f�o�C�X�͂܂������Ă���

					if (hr != D3DERR_DEVICENOTRESET)
						m_pWindow->Exit(); // �G���[

					hr = m_pDevice->Reset(&m_d3dpp); // ���������݂�
					if (FAILED(hr))
					{
						if (hr == D3DERR_DEVICELOST)
							return ; // �f�o�C�X�͂܂������Ă���

						// �G���[
						m_pWindow->Exit();
					}

				}
				// �f�o�C�X����������
				m_IsDeviceLost = false;
			}
		}
	}
}

//EOF