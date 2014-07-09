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
		// プロシージャをひもづけるだけ
		//------------------------------------------------------------------------------
		std::map<HWND, window*> window::_procMap;
		window* window::_firstWindow = nullptr;

		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		window::window(HINSTANCE hInstance, const char * windowName, const char * className, unsigned int width, unsigned int height)
			: _width(width)
			, _height(height)
			, _hInstance(hInstance)
			, _className(className)
			, _IsEnd(false)
		{
			// ウィンドウクラスの作成
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

			// 最初だけ入れておく
			if (_firstWindow == nullptr)
			{
				_firstWindow = this;
			}

			// map
			_procMap.insert(ProcMap::value_type(_hWnd, this));
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		window::~window()
		{
			// 
			auto it = _procMap.find(_hWnd);
			if (it != _procMap.end())
				_procMap.erase(it);

			// 自分がfirstウィンドウだったら
			if (this == _firstWindow)
			{
				_firstWindow = nullptr;	

				// まだウインドウがあったらfirstWindowに追加しておく
				if (!_procMap.empty())
				{
					_firstWindow = _procMap.begin()->second;
				}
			}

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
			SetWindowText(_hWnd, name);
		}

		//==============================================================================
		// ウィンドウプロシージャ
		//------------------------------------------------------------------------------
		LRESULT CALLBACK window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{

			// メッセージ判定
			switch (uMsg)
			{
				// 終了メッセージ
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

			// システムに処理を依頼
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
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
					break;
				}
				else 
				{
					// Updateする
					Update();
				}	
			}	// メッセージループ終了

			Uninit();
			return (int)msg.wParam;
		}
	}
}


//EOF