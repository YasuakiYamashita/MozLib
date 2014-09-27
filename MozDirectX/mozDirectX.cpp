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
#include "mozDirectX.h"
#include <stdio.h>

//------------------------------------------------------------------------------
// �f�o�C�X�擾
//------------------------------------------------------------------------------
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		DirectX::DirectX(moz::window::window* _window, bool bWindowMode)
			: m_bWindowMode(bWindowMode)
			, m_pWindow(_window)
			, m_IsDeviceLost(false)
			, m_fullChenge(false)
		{
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
			if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_d3ddm)))
			{
				// �擾���s
				MessageBox(m_pWindow->GetHWnd(), TEXT("���݂̃f�B�X�v���C���[�h�̎擾�Ɏ��s���܂���"), TEXT("�x��"), MB_OK);
				m_pWindow->Exit();
				return;
			}

			// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
			ZeroMemory(&m_d3dppWindow, sizeof(m_d3dppWindow));						// ���[�N���[���N���A
			m_d3dppWindow.BackBufferWidth				= 0;						// �Q�[����ʃT�C�Y(��)
			m_d3dppWindow.BackBufferHeight				= 0;						// �Q�[����ʃT�C�Y(����)
			m_d3dppWindow.BackBufferFormat				= D3DFMT_UNKNOWN;			// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
			m_d3dppWindow.BackBufferCount				= 1;						// �o�b�N�o�b�t�@�̐�
			m_d3dppWindow.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
			m_d3dppWindow.Windowed						= TRUE;						// �E�B���h�E���[�h
			m_d3dppWindow.hDeviceWindow					= m_pWindow->GetHWnd();		// �E�C���h�E�f�o�C�X
			m_d3dppWindow.EnableAutoDepthStencil		= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
			m_d3dppWindow.AutoDepthStencilFormat		= D3DFMT_D16 ;			// �f�v�X�o�b�t�@�Ƃ���16bit���g��
			m_d3dppWindow.Flags							= 0;						// �t���O
			m_d3dppWindow.MultiSampleType				= D3DMULTISAMPLE_NONE;		// �}���`�T���v��
			m_d3dppWindow.MultiSampleQuality			= 0;						// �}���`�T���v��
			m_d3dppWindow.FullScreen_RefreshRateInHz	= 0;
			m_d3dppWindow.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;

			// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
			ZeroMemory(&m_d3dppFull, sizeof(m_d3dppFull));						// ���[�N���[���N���A
			m_d3dppFull.BackBufferWidth				= m_d3ddm.Width;			// �Q�[����ʃT�C�Y(��)
			m_d3dppFull.BackBufferHeight			= m_d3ddm.Height;			// �Q�[����ʃT�C�Y(����)
			m_d3dppFull.BackBufferFormat			= m_d3ddm.Format;			// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
			m_d3dppFull.BackBufferCount				= 1;						// �o�b�N�o�b�t�@�̐�
			m_d3dppFull.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
			m_d3dppFull.Windowed					= FALSE;					// �E�B���h�E���[�h
			m_d3dppFull.hDeviceWindow				= m_pWindow->GetHWnd();		// �E�C���h�E�f�o�C�X
			m_d3dppFull.EnableAutoDepthStencil		= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
			m_d3dppFull.AutoDepthStencilFormat		= D3DFMT_D16 ;			// �f�v�X�o�b�t�@�Ƃ���16bit���g��
			m_d3dppFull.Flags						= 0;						// �t���O
			m_d3dppFull.MultiSampleType				= D3DMULTISAMPLE_NONE;		// �}���`�T���v��
			m_d3dppFull.MultiSampleQuality			= 0;						// �}���`�T���v��
			m_d3dppFull.FullScreen_RefreshRateInHz	= 0;
			m_d3dppFull.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

			// �|�C���^�ŕ�����
			if (m_bWindowMode)
			{
				m_d3dpp = &m_d3dppWindow;
			}
			else
			{
				m_d3dpp = &m_d3dppFull;
			}

			// �f�o�C�X�I�u�W�F�N�g�̐���
			// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				m_pWindow->GetHWnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				m_d3dpp, &m_pDevice)))
			{
				// �擾���s
				MessageBox(m_pWindow->GetHWnd(), TEXT("�f�o�C�X�I�u�W�F�N�g�̐����Ɏ��s���܂���\r\n��������Â��\��������܂�"), TEXT("�x��"), MB_OK);
				m_pWindow->Exit();
				SAFE_RELEASE(pD3D);
				return;
			}

			// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// ���ʂ��J�����O
			m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);							// Z�o�b�t�@���g�p
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
			m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// ���u�����h�̎��
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��


			// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
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

				// �S��ʃ��[�h�ƃE�B���h�E���[�h��؂�ւ���
				_FlipScreenMode();

				if (!m_IsDeviceLost)
				{
					// 0.5�b���Ƃ�FPS���v��
					if ((dwCurrentTime - dwFPSLastTime) >= kFPSMeasuringSpan)
					{
						fps = dwFrameCount * kInMSecOneSec / (dwCurrentTime - dwFPSLastTime);

						dwFPSLastTime = dwCurrentTime;	// FPS�v���������Ԃ��L�^
						dwFrameCount = 0;				// �b�ԃt���[���������Z�b�g

#ifdef _DEBUG
						// �E�B���h�E����ݒ�
						char cText[256];
						sprintf_s(cText, "%dfps", fps);
						m_pWindow->SetWindowName(cText);
#endif

					}

					// FPS����(�O��̏������ԂƂ̍���(1/FPS)�b�ȏ�̏ꍇ�͏���)
					if ((dwCurrentTime - dwExecLastTime + (kGameFPS - fps)) >= (kInMSecOneSecFloat / kGameFPS))
					{
						// �����J�n���Ԃ��L�^(�����������ɑΉ����邽�߁A�I���ł͂Ȃ��J�n���Ԃ��L�^)
						dwExecLastTime = dwCurrentTime;

						// �A�b�v�f�[�g
						Update();

						// �V�[���̃N���A
						m_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

						// �V�[���̕`��J�n
						if (SUCCEEDED(m_pDevice->BeginScene()))
						{
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

			//���傢�X���[�v
			Sleep(200);

		}
		//==============================================================================
		// �S��ʂƃE�C���h�E��؂�ւ���
		//------------------------------------------------------------------------------
		void DirectX::_FlipScreenMode(void)
		{
			if (!m_fullChenge)
			{
				return;
			}
			m_fullChenge = false;

			// TODO: �f�o�C�X���X�g�������Ƀo�O��B
			// �����Ƃ��̂Ȃ�f�o�C�X���X�g�����ǉ����Ăˁ[

			m_bWindowMode = !m_bWindowMode;
			HWND hWnd = m_pWindow->GetHWnd();

			if (m_bWindowMode)
			{
				// �E�B���h�E���[�h
				m_d3dpp = &m_d3dppWindow;
			}
			else
			{
				// �t���X�N���[�����[�h
				m_d3dpp = &m_d3dppFull;
			}

			// �f�o�C�X���Z�b�g
			HRESULT hr = m_pDevice->Reset(m_d3dpp);
			if (FAILED(hr))
			{
				if (hr == D3DERR_DEVICELOST)
					m_IsDeviceLost = true;
				else
					m_pWindow->Exit();
				return;
			}

			RECT rect;
			GetWindowRect(m_pWindow->GetHWnd(), &rect);

			if (m_bWindowMode)
			{
				SetWindowLong(m_pWindow->GetHWnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX));
				SetWindowPos(m_pWindow->GetHWnd(),
					HWND_NOTOPMOST,                           //�t���X�N���[���͍őO�ʕ\������Ă���̂ŁA��������B
					0,
					0,
					m_pWindow->GetWidth(),
					m_pWindow->GetHeight(),
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
		// �S��ʂƃE�C���h�E��؂�ւ���
		//------------------------------------------------------------------------------
		void DirectX::FlipScreenMode(void)
		{
			m_fullChenge = true;
		}

		//==============================================================================
		// �f�o�C�X�擾����
		//------------------------------------------------------------------------------
		void DirectX::DeviceTryBack(void)
		{
			// �f�o�C�X�����X�g�����ꍇ
			if (m_IsDeviceLost)
			{
				// �f�o�C�X��Ԃ̃`�F�b�N
				HRESULT hr = m_pDevice->TestCooperativeLevel();

				if (FAILED(hr))
				{
					if (hr == D3DERR_DEVICELOST)
						return ;  // �f�o�C�X�͂܂������Ă���

					if (hr != D3DERR_DEVICENOTRESET)
						m_pWindow->Exit(); // �G���[

					// �f�o�C�X���X�g�Ή�
					for (auto it = m_DevLost.begin(); it != m_DevLost.end(); ++it)
					{
						(*it)->Backup();
					}

					hr = m_pDevice->Reset(m_d3dpp); // ���������݂�

					if (FAILED(hr))
					{
						if (hr == D3DERR_DEVICELOST)
							return ; // �f�o�C�X�͂܂������Ă���

						// �G���[
						m_pWindow->Exit();
					}


					// �f�o�C�X���X�g�Ή�
					for (auto it = m_DevLost.begin(); it != m_DevLost.end(); ++it)
					{
						(*it)->Recover( );
					}

				}
				// �f�o�C�X����������
				m_IsDeviceLost = false;
			}
		}

		//==============================================================================
		// �f�o�C�X���X�g���X�g�ǉ�
		//------------------------------------------------------------------------------
		std::list<LostResource*>::iterator DirectX::SetLostResource(LostResource * const resource)
		{
			for (auto it = m_DevLost.begin(); it != m_DevLost.end(); ++it)
			{
				if ((*it) == resource)
				{
					return it;
				}
			}

			return m_DevLost.insert(m_DevLost.end(), resource);
		}

		//==============================================================================
		// �f�o�C�X���X�g���X�g�폜
		//------------------------------------------------------------------------------
		void DirectX::DelLostResource(std::list<LostResource*>::iterator const it)
		{
			m_DevLost.erase(it);
		}
	} 
}

//EOF