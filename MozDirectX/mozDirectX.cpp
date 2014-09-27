//==============================================================================
//
// DirectXのデバイス[mozDirectX.cpp]
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
// デバイス取得
//------------------------------------------------------------------------------
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		DirectX::DirectX(moz::window::window* _window, bool bWindowMode)
			: m_bWindowMode(bWindowMode)
			, m_pWindow(_window)
			, m_IsDeviceLost(false)
			, m_fullChenge(false)
		{
			// Direct3Dオブジェクトの作成
			LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
			if (pD3D == NULL)
			{
				// 作成失敗
				MessageBox(m_pWindow->GetHWnd(), TEXT("Direct3Dオブジェクトの作成に失敗しました"), TEXT("警告"), MB_OK);
				m_pWindow->Exit();
				return;
			}

			// 現在のディスプレイモードを取得
			if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_d3ddm)))
			{
				// 取得失敗
				MessageBox(m_pWindow->GetHWnd(), TEXT("現在のディスプレイモードの取得に失敗しました"), TEXT("警告"), MB_OK);
				m_pWindow->Exit();
				return;
			}

			// デバイスのプレゼンテーションパラメータの設定
			ZeroMemory(&m_d3dppWindow, sizeof(m_d3dppWindow));						// ワークをゼロクリア
			m_d3dppWindow.BackBufferWidth				= 0;						// ゲーム画面サイズ(幅)
			m_d3dppWindow.BackBufferHeight				= 0;						// ゲーム画面サイズ(高さ)
			m_d3dppWindow.BackBufferFormat				= D3DFMT_UNKNOWN;			// バックバッファフォーマットはディスプレイモードに合わせて使う
			m_d3dppWindow.BackBufferCount				= 1;						// バックバッファの数
			m_d3dppWindow.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
			m_d3dppWindow.Windowed						= TRUE;						// ウィンドウモード
			m_d3dppWindow.hDeviceWindow					= m_pWindow->GetHWnd();		// ウインドウデバイス
			m_d3dppWindow.EnableAutoDepthStencil		= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
			m_d3dppWindow.AutoDepthStencilFormat		= D3DFMT_D16 ;			// デプスバッファとして16bitを使う
			m_d3dppWindow.Flags							= 0;						// フラグ
			m_d3dppWindow.MultiSampleType				= D3DMULTISAMPLE_NONE;		// マルチサンプル
			m_d3dppWindow.MultiSampleQuality			= 0;						// マルチサンプル
			m_d3dppWindow.FullScreen_RefreshRateInHz	= 0;
			m_d3dppWindow.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;

			// デバイスのプレゼンテーションパラメータの設定
			ZeroMemory(&m_d3dppFull, sizeof(m_d3dppFull));						// ワークをゼロクリア
			m_d3dppFull.BackBufferWidth				= m_d3ddm.Width;			// ゲーム画面サイズ(幅)
			m_d3dppFull.BackBufferHeight			= m_d3ddm.Height;			// ゲーム画面サイズ(高さ)
			m_d3dppFull.BackBufferFormat			= m_d3ddm.Format;			// バックバッファフォーマットはディスプレイモードに合わせて使う
			m_d3dppFull.BackBufferCount				= 1;						// バックバッファの数
			m_d3dppFull.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
			m_d3dppFull.Windowed					= FALSE;					// ウィンドウモード
			m_d3dppFull.hDeviceWindow				= m_pWindow->GetHWnd();		// ウインドウデバイス
			m_d3dppFull.EnableAutoDepthStencil		= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
			m_d3dppFull.AutoDepthStencilFormat		= D3DFMT_D16 ;			// デプスバッファとして16bitを使う
			m_d3dppFull.Flags						= 0;						// フラグ
			m_d3dppFull.MultiSampleType				= D3DMULTISAMPLE_NONE;		// マルチサンプル
			m_d3dppFull.MultiSampleQuality			= 0;						// マルチサンプル
			m_d3dppFull.FullScreen_RefreshRateInHz	= 0;
			m_d3dppFull.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

			// ポインタで分ける
			if (m_bWindowMode)
			{
				m_d3dpp = &m_d3dppWindow;
			}
			else
			{
				m_d3dpp = &m_d3dppFull;
			}

			// デバイスオブジェクトの生成
			// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				m_pWindow->GetHWnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				m_d3dpp, &m_pDevice)))
			{
				// 取得失敗
				MessageBox(m_pWindow->GetHWnd(), TEXT("デバイスオブジェクトの生成に失敗しました\r\n動作環境が古い可能性があります"), TEXT("警告"), MB_OK);
				m_pWindow->Exit();
				SAFE_RELEASE(pD3D);
				return;
			}

			// レンダーステートパラメータの設定
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// 裏面をカリング
			m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);							// Zバッファを使用
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
			m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// αブレンドの種類
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定


			// サンプラーステートパラメータの設定
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// テクスチャアドレッシング方法(U値)を設定
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// テクスチャアドレッシング方法(V値)を設定
			m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小フィルタモードを設定
			m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大フィルタモードを設定

			// テクスチャステージステートの設定
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数


			// 開放
			SAFE_RELEASE(pD3D);
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		DirectX::~DirectX()
		{
			// デリート
			SAFE_RELEASE(m_pDevice);
		}

		//==============================================================================
		// 動作
		//------------------------------------------------------------------------------
		void DirectX::run(bool* isExit)
		{
			const int	kGameFPS = 60;				// 秒間フレーム数
			const int	kFPSMeasuringSpan = 500;				// FPS計測間隔(ミリ秒)
			const int	kInMSecOneSec = 1000;				// 1秒をミリ秒で表す
			const float kInMSecOneSecFloat = 1000.0f;			// 上記実数版

			// FPS調整用変数
			DWORD dwExecLastTime;	// 最後に処理した時間
			DWORD dwFPSLastTime;	// 最後にFPS計測した時間
			DWORD dwCurrentTime;	// 現在時間
			DWORD dwFrameCount;		// 現在フレーム数
			int fps = 0;			// 現在のFPS

			dwExecLastTime = dwFPSLastTime = timeGetTime();	// 現在時刻を最終処理時間に設定
			dwCurrentTime = dwFrameCount = 0;				// 初期化

			// 初期化処理
			Init();

			while (!(*isExit))
			{
				// 現在時間を取得
				dwCurrentTime = timeGetTime();

				// デバイスを奪い返す
				DeviceTryBack();

				// 全画面モードとウィンドウモードを切り替える
				_FlipScreenMode();

				if (!m_IsDeviceLost)
				{
					// 0.5秒ごとにFPSを計測
					if ((dwCurrentTime - dwFPSLastTime) >= kFPSMeasuringSpan)
					{
						fps = dwFrameCount * kInMSecOneSec / (dwCurrentTime - dwFPSLastTime);

						dwFPSLastTime = dwCurrentTime;	// FPS計測した時間を記録
						dwFrameCount = 0;				// 秒間フレーム数をリセット

#ifdef _DEBUG
						// ウィンドウ名を設定
						char cText[256];
						sprintf_s(cText, "%dfps", fps);
						m_pWindow->SetWindowName(cText);
#endif

					}

					// FPS調整(前回の処理時間との差が(1/FPS)秒以上の場合は処理)
					if ((dwCurrentTime - dwExecLastTime + (kGameFPS - fps)) >= (kInMSecOneSecFloat / kGameFPS))
					{
						// 処理開始時間を記録(処理落ち時に対応するため、終了ではなく開始時間を記録)
						dwExecLastTime = dwCurrentTime;

						// アップデート
						Update();

						// シーンのクリア
						m_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

						// シーンの描画開始
						if (SUCCEEDED(m_pDevice->BeginScene()))
						{
							// 描画処理
							Draw();

							// シーンの描画終了
							m_pDevice->EndScene();
						}

						// シーンの表示
						if (m_pDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
						{
							m_IsDeviceLost = true;
						}

						// 秒間フレーム数を記録
						dwFrameCount++;
					}

				}

				// 処理が早過ぎる場合はちょいっとスリープ
				if (dwCurrentTime == timeGetTime())
				{
					Sleep(1);
				}
			}

			// 終了処理
			Uninit();

			//ちょいスリープ
			Sleep(200);

		}
		//==============================================================================
		// 全画面とウインドウを切り替える
		//------------------------------------------------------------------------------
		void DirectX::_FlipScreenMode(void)
		{
			if (!m_fullChenge)
			{
				return;
			}
			m_fullChenge = false;

			// TODO: デバイスロストした時にバグる。
			// ちゃんとやるのならデバイスロスト処理追加してねー

			m_bWindowMode = !m_bWindowMode;
			HWND hWnd = m_pWindow->GetHWnd();

			if (m_bWindowMode)
			{
				// ウィンドウモード
				m_d3dpp = &m_d3dppWindow;
			}
			else
			{
				// フルスクリーンモード
				m_d3dpp = &m_d3dppFull;
			}

			// デバイスリセット
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
					HWND_NOTOPMOST,                           //フルスクリーンは最前面表示されているので、解除する。
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
		// 全画面とウインドウを切り替える
		//------------------------------------------------------------------------------
		void DirectX::FlipScreenMode(void)
		{
			m_fullChenge = true;
		}

		//==============================================================================
		// デバイス取得処理
		//------------------------------------------------------------------------------
		void DirectX::DeviceTryBack(void)
		{
			// デバイスがロストした場合
			if (m_IsDeviceLost)
			{
				// デバイス状態のチェック
				HRESULT hr = m_pDevice->TestCooperativeLevel();

				if (FAILED(hr))
				{
					if (hr == D3DERR_DEVICELOST)
						return ;  // デバイスはまだ失われている

					if (hr != D3DERR_DEVICENOTRESET)
						m_pWindow->Exit(); // エラー

					// デバイスロスト対応
					for (auto it = m_DevLost.begin(); it != m_DevLost.end(); ++it)
					{
						(*it)->Backup();
					}

					hr = m_pDevice->Reset(m_d3dpp); // 復元を試みる

					if (FAILED(hr))
					{
						if (hr == D3DERR_DEVICELOST)
							return ; // デバイスはまだ失われている

						// エラー
						m_pWindow->Exit();
					}


					// デバイスロスト対応
					for (auto it = m_DevLost.begin(); it != m_DevLost.end(); ++it)
					{
						(*it)->Recover( );
					}

				}
				// デバイスが復元した
				m_IsDeviceLost = false;
			}
		}

		//==============================================================================
		// デバイスロストリスト追加
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
		// デバイスロストリスト削除
		//------------------------------------------------------------------------------
		void DirectX::DelLostResource(std::list<LostResource*>::iterator const it)
		{
			m_DevLost.erase(it);
		}
	} 
}

//EOF