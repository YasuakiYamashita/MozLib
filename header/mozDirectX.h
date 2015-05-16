//==============================================================================
//
// DirectX[mozDirextX.h]
// Author : Yasuaki Yamashita : 2014/07/29
//
//==============================================================================

#pragma once

#ifndef _MOZDIREXTX_H_
#define _MOZDIREXTX_H_
//==============================================================================
// include
//------------------------------------------------------------------------------
#include <Windows.h>
#include <mozWindow.h>
#include <d3dx9.h>
#include <map>
#include <list>
#include <set>
#include <string>

#ifdef _DEBUG
#pragma comment(lib, "lib/mozDirectX_d.lib")
#else
#pragma comment(lib, "lib/mozDirectX.lib")
#endif

//==============================================================================
// 前方宣言
//------------------------------------------------------------------------------
class window;

namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// デバイスロスト用  基底クラス
		//------------------------------------------------------------------------------
		class LostResource
		{
		public:
			virtual void Backup(void) = 0;    // バックアップ
			virtual void Recover(void) = 0;   // 復旧
		};

		//==============================================================================
		// DirectX
		//------------------------------------------------------------------------------
		class DirectX
		{
		public:

			// コンストラクタ
			DirectX(moz::window::window* window, bool bWindowMode = true);
			// デストラクタ
			virtual ~DirectX();

			// デバイス取得
			LPDIRECT3DDEVICE9 GetDevice(void){ return m_pDevice; }

			// 全画面モード と ウインドウモードを切り替える
			void FlipScreenMode(void);

			// 動かす
			// 引数　：　終了フラグを入れておくポインタ
			void run(bool* isExit);

			// デバイスロスト用
			std::list<LostResource*>::iterator SetLostResource(LostResource * const);
			void DelLostResource(std::list<LostResource*>::iterator const);

			// Window取得
			const moz::window::window* GetWindow(void) const { return m_pWindow; }

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

			// デバイスロスト用
			std::list<LostResource*> m_DevLost;

		protected:
			// Windowポインタ
			moz::window::window* m_pWindow;
		};
	}
}
#endif
//EOF