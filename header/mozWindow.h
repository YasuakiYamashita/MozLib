//==============================================================================
//
// Windowクラス[mozWindow.h]
// Author : Yasuaki Yamashita : 2014/07/09
//
//==============================================================================

#pragma once

#ifndef _MOZWINDOW_H_
#define _MOZWINDOW_H_

//==============================================================================
// include
//------------------------------------------------------------------------------
#include <Windows.h>
#include <d3dx9.h>

#ifdef _DEBUG
#pragma comment(lib, "lib/mozWindow_d.lib")
#else
#pragma comment(lib, "lib/mozWindow.lib")
#endif

//==============================================================================
//  
// Window表示クラス
// これを継承させて使ってね！
//
//==============================================================================
namespace moz
{
	namespace window
	{
		//==============================================================================
		// Window表示するだけ
		//------------------------------------------------------------------------------
		class window
		{
		public:


			// コンストラクタ
			window(HINSTANCE hInstance, const char * windowName, const char * className = "mozWindow", unsigned int width = 1280u, unsigned int height = 720u);

			// デストラクタ
			virtual ~window();

			// これを呼ぶと走りだす
			int run(void);

			// ハンドル取得
			inline HWND GetHWnd(void) const { return _hWnd; }
			inline HINSTANCE GetHInstance(void) const { return _hInstance; }

			// サイズ取得
			inline unsigned int GetWidth(void) const { return _width; }
			inline unsigned int GetHeight(void) const { return _height; }

			// ウィンドウの名前を変える
			void SetWindowName(const char *);

			// 終了させる
			void Exit(void){ _IsEnd = true; }

			// アクティブかどうか
			bool GetActive(void){ return m_bActive; }

			// プロシージャ
			virtual void _wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

			// 現在のウインドウモード
			bool GetWindowMode(void){ return _bWindowMode; }

			// フルスクリーンモード
			void FlipWindowMode(void);

		private:
			// ハンドル
			HINSTANCE	_hInstance;

			// hWnd
			HWND		_hWnd;

			// サイズ
			unsigned int _width, _height;

			// windowClass
			const char* _className;

			// 画面がアクティブかどうか
			bool m_bActive;
			void SetActive(bool);

			// 終了フラグ
			bool _IsEnd;

			// フルスクリーンモード
			bool _bWindowMode;

		protected:
			// Update
			virtual void Update(void){};
			virtual void Init(void){};
			virtual void Uninit(void){};
		};

		//==============================================================================
		// DirectX
		//------------------------------------------------------------------------------
		class DirectX
		{
		public:

			// コンストラクタ
			DirectX(window* window, bool bWindowMode = true);
			// デストラクタ
			~DirectX();

			// デバイス取得
			LPDIRECT3DDEVICE9 GetDevice(void){ return m_pDevice; }

			// 全画面モード と ウインドウモードを切り替える
			void FlipScreenMode(void);

			// 動かす
			// 引数　：　終了フラグを入れておくポインタ
			void run(bool* isExit);

		protected:
			// オーバーライドすれば読み込むよー
			virtual void Init(void)  {};
			virtual void Update(void){};
			virtual void Draw(void)  {};
			virtual void Uninit(void){};

		private:
			// デバイス復元処理
			void DeviceTryBack(void);
			
			// Deviceオブジェクト(描画に必要)
			LPDIRECT3DDEVICE9		m_pDevice;

			// スクリーンモード
			bool m_bWindowMode;
			D3DPRESENT_PARAMETERS m_d3dppFull;
			D3DPRESENT_PARAMETERS m_d3dppWindow;
			D3DPRESENT_PARAMETERS *m_d3dpp;

			// 今のディスプレイモード
			D3DDISPLAYMODE m_d3ddm;

			// DirectXデバイスLOSTフラグ
			bool m_IsDeviceLost;

			// フラグ
			void _FlipScreenMode(void);
			
			// フルスクリーンに変えるフラグ
			bool m_fullChenge;

			// Windowポインタ
			window* m_pWindow;
		};
	}
}


#endif
//EOF