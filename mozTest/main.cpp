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
#include <mozMath.h>
#include <mozDirectX.h>
#include <mozPolygon.h>
#include <mozTexture.h>
#include <moz3DPolygon.h>
#include <mozInput.h>

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

using namespace moz::math;

//==============================================================================
// DirectX
//------------------------------------------------------------------------------
class testDirectX : public moz::DirectX::DirectX 
{
public:
	testDirectX(moz::window::window* window, bool bWindowMode = true) :moz::DirectX::DirectX(window, bWindowMode){};
	~testDirectX(){};
protected:
	void Init(void)
	{
		m_PolygonManager = new moz::DirectX::PolygonManager(this);
		m_TexManager = new moz::DirectX::TextureManager(this);
		m_InputManager = new moz::input::InputManager(m_pWindow);
		
		m_TexManager->LoadDirectory("./data/TEXTURE/test", "test");

		m_PolygonManager->Create2D(Vector2D(400,400))->GetPos() = Vector3D(200,200,0);

		moz::DirectX::Polygon3D* polygon = nullptr;
		m_PolygonManager->Create3D<moz::DirectX::PolygonIndex, int, int, float, float>(4, 4, 20.f, 20.f)->GetTex() = m_TexManager->GetTex("field003");
		
		polygon = m_PolygonManager->Create3D<moz::DirectX::PolygonIndex, int, int, float, float>(1, 1, 3.f, 3.f);
		polygon->GetTex() = m_TexManager->GetTex("field000");
		polygon->GetPos() = Vector3D(0, 1, 0);


		polygon = m_PolygonManager->Create3D<moz::DirectX::PolygonIndex, int, int, float, float>(1, 1, 3.f, 3.f);
		polygon->GetTex() = m_TexManager->GetTex("field000");
		polygon->GetPos() = Vector3D(0, 2, 2);

		polygon = m_PolygonManager->Create3D<moz::DirectX::PolygonIndex2, int, int, float, float>(1, 10, 3.f, 3.f);
		polygon->GetTex() = m_TexManager->GetTex("field000");
		polygon->GetPos() = Vector3D(0, 0.5f, -2);



	}

	void Draw(void)
	{
		m_PolygonManager->Draw();
	}

	void Update(void)
	{
		m_InputManager->Update();

		if (m_InputManager->GetKey()->GetPress(DIK_LEFT))
		{
			m_PolygonManager->GetCamPos() += D3DXVECTOR3(-0.1f, 0, 0);
		}
		if (m_InputManager->GetKey()->GetPress(DIK_RIGHT))
		{
			m_PolygonManager->GetCamPos() += D3DXVECTOR3(0.1f, 0, 0);
		}
		if (m_InputManager->GetKey()->GetPress(DIK_UP))
		{
			m_PolygonManager->GetCamPos() += D3DXVECTOR3(0, 0, 0.1f);
		}
		if (m_InputManager->GetKey()->GetPress(DIK_DOWN))
		{
			m_PolygonManager->GetCamPos() += D3DXVECTOR3(0, 0, -0.1f);
		}



		m_PolygonManager->Update();
	}

	void Uninit(void)
	{
		SAFE_DELETE(m_PolygonManager);
		SAFE_DELETE(m_TexManager);
		SAFE_DELETE(m_InputManager);
	}

	moz::DirectX::PolygonManager* m_PolygonManager;
	moz::DirectX::TextureManager* m_TexManager;

	moz::input::InputManager* m_InputManager;
};

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
		_thread = new moz::thread::Thread(this, false, this);
	}

	DWORD _Func(moz::thread::funcData* data)
	{
		moz::window::window * _wind = (moz::window::window*)data->data;
		m_directx = new testDirectX(_wind, true);

		// 開始
		m_directx->run(&data->isExit);

		// 終了
		SAFE_DELETE(m_directx);

		Sleep(16);

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


protected:
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

	testDirectX* m_directx;
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
