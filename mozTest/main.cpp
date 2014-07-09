//==============================================================================
//
// �e�X�g�v���O����[main.cpp]
// Author : Yasuaki Yamashita : 2014/07/09
//
//------------------------------------------------------------------------------

//==============================================================================
// include
//------------------------------------------------------------------------------
#include <mozWindow.h>
#include <mozThread.h>

#pragma comment(lib, "winmm.lib")

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
		_thread = new moz::thread::Thread(this);
	}

	DWORD _Func(moz::thread::funcData* data)
	{
		while (!data->isExit)
		{
			Sleep(1);
		}

		return 0;
	}

	void Update()
	{
		if (_thread->IsDead())
			SetWindowName("Thread��������`");
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
				Exit();
#else
				{
					int nData = MessageBox(hWnd, TEXT("�I�����܂����H"), TEXT("�I���m�F"), MB_OKCANCEL | MB_ICONQUESTION);
					if (nData == IDOK)
					{
						// �E�B���h�E�폜
						_endFlag = true;
					}
				}
#endif
				break;
			case VK_RETURN:
				_thread->Exit();
				break;
			}
		}
	}
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