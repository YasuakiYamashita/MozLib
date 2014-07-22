//==============================================================================
//
// ウインドウ表示する[mozWindow.cpp]
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
		// コンストラクタ
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
			// Windowsプロシージャ
			//------------------------------------------------------------------------------
			WNDPROC func = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
			{
				window * _window = (window *)GetWindowLong(hWnd, GWL_USERDATA);

				// メッセージ判定
				switch (uMsg)
				{
					// 終了メッセージ
				case WM_DESTROY:
					PostQuitMessage(0);
					break;

				case WM_ACTIVATE:
					_window->SetActive(LOWORD(wParam) != 0);
					
					// 全面表示を解除
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

				// システムに処理を依頼
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			};

			// ウィンドウクラスの作成
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

			// ウィンドウクラスの登録
			RegisterClassEx(&wcex);

			// 適正なウィンドウのサイズを求める
			// ウィンドウモード
			const DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

			RECT windowRect = { 0, 0, _width, _height };
			AdjustWindowRect(&windowRect, windowStyle, FALSE);

			// ウィンドウの作成
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

			//フレームカウント初期化
			// 分解能を設定
			timeBeginPeriod(1);

			// ウインドウの表示(初期化処理の後に呼ばないと駄目)
			ShowWindow(_hWnd, SW_SHOWDEFAULT);
			UpdateWindow(_hWnd);

			// thisを登録しておく
			SetWindowLong(_hWnd, GWL_USERDATA, (LONG)this);
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		window::~window()
		{
			// 自分のポインター解除
			SetWindowLong(_hWnd, GWL_USERDATA, (LONG)nullptr);

			// ウィンドウクラスの登録を解除
			UnregisterClass(_className, _hInstance);

			// 分解能を戻す
			timeEndPeriod(1);
		}

		//==============================================================================
		// ウインドウの名前を変える
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

			// メッセージループ
			for (;;)
			{
				// Winメッセージの取得
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{// PostQuitMessage()が呼ばれたらループ終了
						break;
					}
					else
					{
						// メッセージの翻訳とディスパッチ
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
					// Updateする
					Update();
					Sleep(1);
				}	
			}	// メッセージループ終了

			return (int)msg.wParam;
		}

		//==============================================================================
		// フルスクリーンとウインドウモード切り替え
		//------------------------------------------------------------------------------
		void window::FlipWindowMode(void)
		{

			//==============================================================
			// DirectXの機能を使っていないので健康的
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