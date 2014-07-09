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
		// �v���V�[�W�����Ђ��Â��邾��
		//------------------------------------------------------------------------------
		std::map<HWND, window*> window::_procMap;
		window* window::_firstWindow = nullptr;

		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		window::window(HINSTANCE hInstance, const char * windowName, const char * className, unsigned int width, unsigned int height)
			: _width(width)
			, _height(height)
			, _hInstance(hInstance)
			, _className(className)
			, _IsEnd(false)
		{
			// �E�B���h�E�N���X�̍쐬
			WNDCLASSEX wcex =
			{
				sizeof(WNDCLASSEX),
				CS_CLASSDC,
				WndProc,
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

			// �ŏ���������Ă���
			if (_firstWindow == nullptr)
			{
				_firstWindow = this;
			}

			// map
			_procMap.insert(ProcMap::value_type(_hWnd, this));
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		window::~window()
		{
			// 
			auto it = _procMap.find(_hWnd);
			if (it != _procMap.end())
				_procMap.erase(it);

			// ������first�E�B���h�E��������
			if (this == _firstWindow)
			{
				_firstWindow = nullptr;	

				// �܂��E�C���h�E����������firstWindow�ɒǉ����Ă���
				if (!_procMap.empty())
				{
					_firstWindow = _procMap.begin()->second;
				}
			}

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
			SetWindowText(_hWnd, name);
		}

		//==============================================================================
		// �E�B���h�E�v���V�[�W��
		//------------------------------------------------------------------------------
		LRESULT CALLBACK window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{

			// ���b�Z�[�W����
			switch (uMsg)
			{
				// �I�����b�Z�[�W
			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			}

			if (_procMap.size() == 1 && _firstWindow)
			{
				_firstWindow->_wndProc(hWnd, uMsg, wParam, lParam);
			}

			else
			{
				auto it = _procMap.find(hWnd);

				if (it != _procMap.end())
				{
					it->second->_wndProc(hWnd, uMsg, wParam, lParam);
				}
			}

			// �V�X�e���ɏ������˗�
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
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
					break;
				}
				else 
				{
					// Update����
					Update();
				}	
			}	// ���b�Z�[�W���[�v�I��

			Uninit();
			return (int)msg.wParam;
		}
	}
}


//EOF