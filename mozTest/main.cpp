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


#if TRUE

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
				m_directx->FlipScreenMode();
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

#else

//----------------------------------------------------------
// フルスクリーン切り替え ( Alt + Enter )
//--------------------------------------------------------------

#include <windows.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

#define RELEASE(x) if(x){x->Release();x=NULL;}

HINSTANCE g_hInstance = NULL; // インスタンス・ハンドル
HWND  g_hWindow = NULL; // ウインドウ・ハンドル
HMENU  g_hMenu = NULL; // メニュー・ハンドル

LPCSTR  g_szAppTitle = TEXT("DirectX9 Screen Change");
LPCSTR  g_szWndClass = TEXT("Window1");

RECT  g_rectWindow;       // ウインドウ・モードでの最後の位置

// 起動時の描画領域サイズ
bool  g_bWindow = true;      // 起動時の画面モード

SIZE  g_sizeWindowMode = { 640, 480 };  // ウインドウ・モード
SIZE  g_sizeFullMode = { 640, 480 };  // フルスクリーン・モード
D3DFORMAT g_formatFull = D3DFMT_X8R8G8B8;  // ディスプレイ(バック・バッファ)・フォーマット

// アプリケーションの動作フラグ
bool  g_bActive = false; // アクティブ状態

#define DXTRACE_ERR(x,y) 1;

/*-------------------------------------------
グローバル変数(DirectX関連)
--------------------------------------------*/

// インターフェイス
LPDIRECT3D9    g_pD3D = NULL; // Direct3Dインターフェイス
LPDIRECT3DDEVICE9  g_pD3DDevice = NULL; // Direct3DDeviceインターフェイス
D3DPRESENT_PARAMETERS g_D3DPP;    // D3DDeviceの設定(現在)

D3DPRESENT_PARAMETERS g_D3DPPWindow;   // D3DDeviceの設定(ウインドウ・モード用)
D3DPRESENT_PARAMETERS g_D3DPPFull;   // D3DDeviceの設定(フルスクリーン・モード用)

bool g_bDeviceLost = false;      // デバイスの消失フラグ

/*-------------------------------------------
関数定義
--------------------------------------------*/

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam);

/*-------------------------------------------
アプリケーション初期化（最初に一度だけ呼ばれる）
--------------------------------------------*/
HRESULT InitApp(HINSTANCE hInst)
{
	// アプリケーションのインスタンス・ハンドルを保存
	g_hInstance = hInst;

	// IMEを禁止する
	//ImmDisableIME(-1); // このスレッドで禁止

	// ウインドウ・クラスの登録
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_szWndClass;

	if (!RegisterClassEx(&wcex))
		return DXTRACE_ERR("InitApp", GetLastError());

	// メイン・ウインドウ作成
	g_rectWindow.top = 0;
	g_rectWindow.left = 0;
	g_rectWindow.right = g_sizeWindowMode.cx;
	g_rectWindow.bottom = g_sizeWindowMode.cy;
	AdjustWindowRect(&g_rectWindow, WS_OVERLAPPEDWINDOW, FALSE);
	g_rectWindow.right = g_rectWindow.right - g_rectWindow.left;
	g_rectWindow.bottom = g_rectWindow.bottom - g_rectWindow.top;
	g_rectWindow.top = 0;
	g_rectWindow.left = 0;

	RECT rect;
	if (g_bWindow)
	{
		// (ウインドウ・モード用)
		rect.top = CW_USEDEFAULT;
		rect.left = CW_USEDEFAULT;
		rect.right = g_rectWindow.right;
		rect.bottom = g_rectWindow.bottom;
	}
	else
	{
		// (フルスクリーン・モード用)
		rect.top = 0;
		rect.left = 0;
		rect.right = g_sizeFullMode.cx;
		rect.bottom = g_sizeFullMode.cy;

		//g_hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU1));
	}

	g_hWindow = CreateWindow(g_szWndClass, g_szAppTitle,
		g_bWindow ? WS_OVERLAPPEDWINDOW : WS_POPUP,
		rect.left, rect.top, rect.right, rect.bottom,
		NULL, NULL, hInst, NULL);
	if (g_hWindow == NULL)
		return DXTRACE_ERR("InitApp", GetLastError());

	// ウインドウ表示
	ShowWindow(g_hWindow, SW_SHOWNORMAL);
	UpdateWindow(g_hWindow);

	return S_OK;
}

/*-------------------------------------------
DirectX Graphics初期化
--------------------------------------------*/
HRESULT InitDXGraphics(void)
{
	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
		return DXTRACE_ERR("InitDXGraphics Direct3DCreate9", E_FAIL);

	// D3DDeviceオブジェクトの設定(ウインドウ・モード用)
	ZeroMemory(&g_D3DPPWindow, sizeof(g_D3DPPWindow));

	g_D3DPPWindow.BackBufferWidth = 0;
	g_D3DPPWindow.BackBufferHeight = 0;
	g_D3DPPWindow.BackBufferFormat = D3DFMT_UNKNOWN;
	g_D3DPPWindow.BackBufferCount = 1;
	g_D3DPPWindow.MultiSampleType = D3DMULTISAMPLE_NONE;
	g_D3DPPWindow.MultiSampleQuality = 0;
	g_D3DPPWindow.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_D3DPPWindow.hDeviceWindow = g_hWindow;
	g_D3DPPWindow.Windowed = TRUE;
	g_D3DPPWindow.EnableAutoDepthStencil = FALSE;
	g_D3DPPWindow.AutoDepthStencilFormat = D3DFMT_A1R5G5B5;
	g_D3DPPWindow.Flags = 0;
	g_D3DPPWindow.FullScreen_RefreshRateInHz = 0;
	g_D3DPPWindow.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// D3DDeviceオブジェクトの設定(フルスクリーン・モード)
	ZeroMemory(&g_D3DPPFull, sizeof(g_D3DPPFull));

	g_D3DPPFull.BackBufferWidth = g_sizeFullMode.cx;
	g_D3DPPFull.BackBufferHeight = g_sizeFullMode.cy;
	g_D3DPPFull.BackBufferFormat = g_formatFull;
	g_D3DPPFull.BackBufferCount = 1;
	g_D3DPPFull.MultiSampleType = D3DMULTISAMPLE_NONE;
	g_D3DPPFull.MultiSampleQuality = 0;
	g_D3DPPFull.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_D3DPPFull.hDeviceWindow = g_hWindow;
	g_D3DPPFull.Windowed = FALSE;
	g_D3DPPFull.EnableAutoDepthStencil = FALSE;
	g_D3DPPFull.AutoDepthStencilFormat = D3DFMT_A1R5G5B5;
	g_D3DPPFull.Flags = 0;
	g_D3DPPFull.FullScreen_RefreshRateInHz = 0;
	g_D3DPPFull.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// D3DDeviceオブジェクトの作成
	if (g_bWindow)
		g_D3DPP = g_D3DPPWindow;
	else
		g_D3DPP = g_D3DPPFull;

	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_D3DPP, &g_pD3DDevice);
	if (FAILED(hr))
	{
		hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, g_hWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3DPP, &g_pD3DDevice);
		if (FAILED(hr))
			return DXTRACE_ERR("InitDXGraphics CreateDevice", hr);
	}

	// ビューポートの設定
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = g_D3DPP.BackBufferWidth;
	vp.Height = g_D3DPP.BackBufferHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	hr = g_pD3DDevice->SetViewport(&vp);
	if (FAILED(hr))
		return DXTRACE_ERR("InitDXGraphics SetViewport", hr);

	return S_OK;
}


/*--------------------------------------------
画面の描画処理
--------------------------------------------*/
HRESULT Render(void)
{
	// シーンのクリア
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	// シーンの描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		// シーンの描画終了
		g_pD3DDevice->EndScene();
	}

	// シーンの表示
	return g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}



/*-------------------------------------------
ウインドウ・サイズの変更
--------------------------------------------*/
HRESULT ChangeWindowSize(void)
{
	// ウインドウのクライアント領域に合わせる

	HRESULT hr = g_pD3DDevice->Reset(&g_D3DPP);
	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICELOST)
			g_bDeviceLost = true;
		else
			DestroyWindow(g_hWindow);
		return DXTRACE_ERR("ChangeWindowSize Reset", hr);
	}

	// ビューポートの設定
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = g_D3DPP.BackBufferWidth;
	vp.Height = g_D3DPP.BackBufferHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	hr = g_pD3DDevice->SetViewport(&vp);
	if (FAILED(hr))
	{
		DXTRACE_ERR("ChangeWindowSize SetViewport", hr);
		DestroyWindow(g_hWindow);
	}
	return hr;
}

/*-------------------------------------------
画面モードの変更
--------------------------------------------*/
void ChangeDisplayMode(void)
{
	g_bWindow = !g_bWindow;

	if (g_bWindow)
	{
		g_D3DPP = g_D3DPPWindow;
	}
	else
	{
		g_D3DPP = g_D3DPPFull;
		GetWindowRect(g_hWindow, &g_rectWindow);
	}

	HRESULT hr = g_pD3DDevice->Reset(&g_D3DPP);
	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICELOST)
			g_bDeviceLost = true;
		else
			DestroyWindow(g_hWindow);
		DXTRACE_ERR("ChangeDisplayMode Reset", hr);
		return;
	}


	if (g_bWindow)
	{
		SetWindowLong(g_hWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		if (g_hMenu != NULL)
		{
			SetMenu(g_hWindow, g_hMenu);
			g_hMenu = NULL;
		}
		SetWindowPos(g_hWindow, HWND_NOTOPMOST,
			g_rectWindow.left, g_rectWindow.top,
			g_rectWindow.right - g_rectWindow.left,
			g_rectWindow.bottom - g_rectWindow.top,
			SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowLong(g_hWindow, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		if (g_hMenu == NULL)
		{
			g_hMenu = GetMenu(g_hWindow);
			SetMenu(g_hWindow, NULL);
		}
	}
}

/*-------------------------------------------
DirectX Graphicsの終了処理(初期化に失敗したときも呼ばれる)
--------------------------------------------*/
bool CleanupDXGraphics(void)
{

	RELEASE(g_pD3DDevice);
	RELEASE(g_pD3D);

	return true;
}

/*-------------------------------------------
アプリケーションの終了処理（最後に呼ばれる）
--------------------------------------------*/
bool CleanupApp(void)
{
	// メニュー・ハンドルの削除
	if (g_hMenu)
		DestroyMenu(g_hMenu);

	// ウインドウ・クラスの登録解除
	UnregisterClass(g_szWndClass, g_hInstance);
	return true;
}

/*-------------------------------------------
ウィンドウ処理
--------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	HRESULT hr = S_OK;

	switch (msg)
	{
	case WM_ACTIVATE:
		g_bActive = (LOWORD(wParam) != 0);
		break;

	case WM_DESTROY:
		// DirectX Graphicsの終了処理
		CleanupDXGraphics();
		// ウインドウを閉じる
		PostQuitMessage(0);
		g_hWindow = NULL;
		return 0;

		// ウインドウ・サイズの変更処理
	case WM_SIZE:
		if (g_D3DPP.Windowed != TRUE)
			break;

		if (!g_pD3DDevice || wParam == SIZE_MINIMIZED)
			break;
		g_D3DPP.BackBufferWidth = LOWORD(lParam);
		g_D3DPP.BackBufferHeight = HIWORD(lParam);
		if (g_bDeviceLost)
			break;
		if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
			ChangeWindowSize();
		break;

	case WM_SETCURSOR:
		if (g_D3DPP.Windowed != TRUE)
		{
			SetCursor(NULL);
			return 1;
		}
		break;

	case WM_KEYDOWN:
		// キー入力の処理
		switch (wParam)
		{
		case VK_ESCAPE: // [ESCAPE]キーでウインドウを閉じる
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		break;
	case WM_SYSKEYDOWN:     // Alt + 特殊キーの処理に使う
		switch (wParam)
		{
		case VK_RETURN:     // Alt + Enterを押すと切り替え
			ChangeDisplayMode();
			break;
		default:
			break;
		}
		break;
	}

	// デフォルト処理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*--------------------------------------------
アイドル時の処理
--------------------------------------------*/
bool AppIdle(void)
{
	if (!g_pD3D || !g_pD3DDevice)
		return false;

	if (!g_bActive)
		return true;

	// 消失したデバイスの復元処理
	HRESULT hr;
	if (g_bDeviceLost)
	{
		Sleep(100); // 0.1秒待つ

		// デバイス状態のチェック
		hr = g_pD3DDevice->TestCooperativeLevel();
		if (FAILED(hr))
		{
			if (hr == D3DERR_DEVICELOST)
				return true;  // デバイスはまだ失われている

			if (hr != D3DERR_DEVICENOTRESET)
				return false; // 予期せぬエラー

			hr = g_pD3DDevice->Reset(&g_D3DPP); // 復元を試みる
			if (FAILED(hr))
			{
				if (hr == D3DERR_DEVICELOST)
					return true; // デバイスはまだ失われている

				DXTRACE_ERR("AppIdle Reset", hr);
				return false; // デバイスの復元に失敗
			}

		}
		// デバイスが復元した
		g_bDeviceLost = false;
	}

	// 画面の更新
	hr = Render();
	if (hr == D3DERR_DEVICELOST)
		g_bDeviceLost = true; // デバイスの消失
	else if (FAILED(hr))
		return false;

	return true;
}

/*--------------------------------------------
メイン
---------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	// アプリケーションに関する初期化
	HRESULT hr = InitApp(hInst);
	if (FAILED(hr))
	{
		DXTRACE_ERR("WinMain InitApp", hr);
		return 0;
	}

	// DirectX Graphicsの初期化
	hr = InitDXGraphics();
	if (FAILED(hr))
		DXTRACE_ERR("WinMain InitDXGraphics", hr);


	// メッセージ・ループ
	MSG msg;
	do
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// アイドル処理
			if (!AppIdle())
				// エラーがある場合，アプリケーションを終了する
				DestroyWindow(g_hWindow);
		}
	} while (msg.message != WM_QUIT);

	// アプリケーションの終了処理
	CleanupApp();

	return msg.wParam;
}

#endif