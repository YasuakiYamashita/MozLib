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
	}
}


#endif
//EOF