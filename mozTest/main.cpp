//==============================================================================
//
// テストプログラム[main.cpp]
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
// ライブラリのリンク
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

		// 開始
		m_directx->run(&data->isExit);
		
		// 終了
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
			// キー入力メッセージ
		case WM_KEYDOWN:

			// パラメータ判定
			switch (wParam)
			{
				// エスケープキー
			case VK_ESCAPE:
#ifdef _DEBUG
				_thread->Exit();
#else
				{
					int nData = MessageBox(hWnd, TEXT("終了しますか？"), TEXT("終了確認"), MB_OKCANCEL | MB_ICONQUESTION);
					if (nData == IDOK)
					{
						// ウィンドウ削除
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
		case WM_SYSKEYDOWN:     // Alt + 特殊キーの処理に使う
			switch (wParam)
			{
			case VK_RETURN:     // Alt + Enterを押すと切り替え
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
