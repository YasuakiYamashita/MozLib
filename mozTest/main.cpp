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

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

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
		_thread = new moz::thread::Thread(this,false,this);
	}

	DWORD _Func(moz::thread::funcData* data)
	{
		moz::window::window * _wind = (moz::window::window*)data->data;
		m_directx = new moz::window::DirectX(_wind, true);

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


protected :
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

	moz::window::DirectX* m_directx;
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
