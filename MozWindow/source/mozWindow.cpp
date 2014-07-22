//==============================================================================
//
// �E�C���h�E�\������[mozWindow.cpp]
// Author : Yasuaki Yamashita : 2014/07/09
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "mozWindow.h"

namespace moz
{
	namespace window
	{
		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		window::window(HINSTANCE hInstance, const char * windowName, const char * className, unsigned int width, unsigned int height)
			: _width(width)
			, _height(height)
			, _hInstance(hInstance)
			, _className(className)
			, _IsEnd(false)
			, _bWindowMode(false)
		{
			//==============================================================================
			// Windows�v���V�[�W��
			//------------------------------------------------------------------------------
			WNDPROC func = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
			{
				window * _window = (window *)GetWindowLong(hWnd, GWL_USERDATA);

				// ���b�Z�[�W����
				switch (uMsg)
				{
					// �I�����b�Z�[�W
				case WM_DESTROY:
					PostQuitMessage(0);
					break;

				case WM_ACTIVATE:
					_window->SetActive(LOWORD(wParam) != 0);
					
					// �S�ʕ\��������
					if (!_window->GetActive() && _window->GetWindowMode())
					{
						_window->FlipWindowMode();
					}
					
					break;
				}

				if (_window)
				{
					_window->_wndProc(hWnd, uMsg, wParam, lParam);
				}

				// �V�X�e���ɏ������˗�
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			};

			// �E�B���h�E�N���X�̍쐬
			WNDCLASSEX wcex =
			{
				sizeof(WNDCLASSEX),
				CS_CLASSDC,
				func,
				0,
				0,
				hInstance,
				NULL,
				LoadCursor(NULL, IDC_ARROW),
				(HBRUSH)(COLOR_WINDOW + 1),
				NULL,
				className,
				NULL
			};

			// �E�B���h�E�N���X�̓o�^
			RegisterClassEx(&wcex);

			// �K���ȃE�B���h�E�̃T�C�Y�����߂�
			// �E�B���h�E���[�h
			const DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

			RECT windowRect = { 0, 0, _width, _height };
			AdjustWindowRect(&windowRect, windowStyle, FALSE);

			// �E�B���h�E�̍쐬
			_hWnd = CreateWindowEx(0,
				className,
				windowName,
				windowStyle,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				windowRect.right - windowRect.left, 
				windowRect.bottom - windowRect.top, 
				NULL,
				NULL,
				hInstance,
				NULL);

			//�t���[���J�E���g������
			// ����\��ݒ�
			timeBeginPeriod(1);

			// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
			ShowWindow(_hWnd, SW_SHOWDEFAULT);
			UpdateWindow(_hWnd);

			// this��o�^���Ă���
			SetWindowLong(_hWnd, GWL_USERDATA, (LONG)this);
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		window::~window()
		{
			// �����̃|�C���^�[����
			SetWindowLong(_hWnd, GWL_USERDATA, (LONG)nullptr);

			// �E�B���h�E�N���X�̓o�^������
			UnregisterClass(_className, _hInstance);

			// ����\��߂�
			timeEndPeriod(1);
		}

		//==============================================================================
		// �E�C���h�E�̖��O��ς���
		//------------------------------------------------------------------------------
		void window::SetWindowName(const char* name)
		{
			if (!_IsEnd)
			{
				SetWindowText(_hWnd, name);
			}
		}


		//==============================================================================
		// SetActive
		//------------------------------------------------------------------------------
		void window::SetActive(bool active)
		{
			m_bActive = active;
		}

		//==============================================================================
		// run
		//------------------------------------------------------------------------------
		int window::run(void)
		{
			MSG msg;

			Init();

			// ���b�Z�[�W���[�v
			for (;;)
			{
				// Win���b�Z�[�W�̎擾
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
						break;
					}
					else
					{
						// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else if (_IsEnd)
				{
					Uninit();
					PostQuitMessage(0);
				}
				else 
				{
					// Update����
					Update();
					Sleep(1);
				}	
			}	// ���b�Z�[�W���[�v�I��

			return (int)msg.wParam;
		}

		//==============================================================================
		// �t���X�N���[���ƃE�C���h�E���[�h�؂�ւ�
		//------------------------------------------------------------------------------
		void window::FlipWindowMode(void)
		{

			//==============================================================
			// DirectX�̋@�\���g���Ă��Ȃ��̂Ō��N�I
			//==============================================================

			if (!_bWindowMode)
			{
				DEVMODE devMod;
				ZeroMemory(&devMod, sizeof(devMod));
				devMod.dmSize = sizeof(devMod);
				devMod.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY | DM_DISPLAYFLAGS;
				devMod.dmPelsWidth = GetSystemMetrics(SM_CXSCREEN);
				devMod.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);
				devMod.dmBitsPerPel = 32;
				devMod.dmDisplayFrequency = 60;
				devMod.dmDisplayFlags = 0;

				int hr = ChangeDisplaySettingsEx(NULL, &devMod, NULL, CDS_TEST | CDS_FULLSCREEN, NULL);

				if (hr == DISP_CHANGE_SUCCESSFUL)
				{
					ChangeDisplaySettingsEx(NULL, &devMod, NULL, CDS_FULLSCREEN, NULL);
				}
				else
				{
					_bWindowMode = false;
					return;
				}

				SetWindowLong(_hWnd, GWL_STYLE, GetWindowLong(_hWnd, GWL_STYLE) & ~(WS_CAPTION | WS_BORDER | WS_THICKFRAME));
				SetWindowLong(_hWnd, GWL_EXSTYLE, GetWindowLong(_hWnd, GWL_EXSTYLE) & ~(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE));
				SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0, devMod.dmPelsWidth, devMod.dmPelsHeight, SWP_SHOWWINDOW);
				_bWindowMode = true;
			}
			// Window mode
			else
			{
				ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL);
				SetWindowLong(_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX));
				SetWindowLong(_hWnd, GWL_EXSTYLE, GetWindowLong(_hWnd, GWL_EXSTYLE) | (WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE));
				SetWindowPos(_hWnd, HWND_NOTOPMOST, 0, 0, _width, _height, SWP_SHOWWINDOW);
				_bWindowMode = false;
			}
		}
	}
}


//EOF