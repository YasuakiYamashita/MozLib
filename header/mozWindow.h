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
#include <map>

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
		class window
		{
		public:

			// 別名
			typedef std::map<HWND, window*> ProcMap;

			// コンストラクタ
			window(HINSTANCE hInstance, const char * windowName, const char * className = "mozWindow", unsigned int width = 1280, unsigned int height = 720);

			// デストラクタ
			virtual ~window();

			// これを呼ぶ
			int run(void);

			// プロシージャ
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			// ハンドル取得
			inline HWND GetHWnd(void){ return _hWnd; }
			inline HINSTANCE GetHInstance(void){ return _hInstance; }

			// サイズ取得
			inline unsigned int GetWidth(void){ return _width; }
			inline unsigned int GetHeight(void){ return _height; }

			// ウィンドウの名前を変える
			void SetWindowName(const char *);

		private:
			// ハンドル
			HINSTANCE	_hInstance;

			// hWnd
			HWND		_hWnd;

			// サイズ
			unsigned int _width, _height;

			// windowClass
			const char* _className;

			// hWndとプロシージャの対応づけ
			static ProcMap _procMap;
			static window* _firstWindow;

			// 終了フラグ
			bool _IsEnd;

		protected:
			// プロシージャ
			virtual void _wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

			// Update
			virtual void Update(void){};
			virtual void Init(void){};
			virtual void Uninit(void){};

			// 終了させる
			void Exit(void){ _IsEnd = true; }
		};
	}
}


#endif
//EOF