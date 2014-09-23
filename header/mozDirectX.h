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
#include <d3dx9.h>
#include <map>
#include <list>
#include <string>
#include <mozWindow.h>

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

		//==============================================================================
		// TexContainer
		//------------------------------------------------------------------------------
		class TexContainer
		{
		public:
			// テクスチャ取得
			const LPDIRECT3DTEXTURE9& GetTex(void){ return _tex; }

		protected:
			TexContainer():_tex(nullptr){};
			virtual ~TexContainer(){};

			// テクスチャ実態
			LPDIRECT3DTEXTURE9 _tex;
		};

		//==============================================================================
		// TextureManager
		//------------------------------------------------------------------------------
		class TextureManager : public LostResource
		{
		private:
			// テクスチャコンテナ
			class _texContainer : public TexContainer
			{
			public:
				_texContainer(const char *TexPath, const char *TexName, const TextureManager * const TexManager, const char *GropName = "general");
				virtual ~_texContainer();

				// Geter
				const std::string& GetGropName(void){ return _sGropName; }
				const std::string& GetTexName(void){ return _sTexName; }

			private:
				const std::string _sGropName;
				const std::string _sTexName;
			};

		public:
			// テクスチャリスト
			typedef std::map<std::string, _texContainer*> texlist;

			TextureManager(DirectX* _directX);
			~TextureManager();

			// テクスチャ作成
			const TexContainer* LoadTex(const char * FilePath, const char * GropName = "general");

			// テクスチャディレクトリ
			void LoadDirectory(const char * Path, const char * GropName);

			// 同じグループを削除
			void DeleteGrop(const char * Grop);

			// ファイルテクスチャ取得
			TexContainer* GetTex(const char * name);

			// テクスチャ
			void Backup(void){};

			// テクスチャ
			void Recover(void){};

		private:
			// データリスト
			texlist _Texlist;

			// DirectXクラス
			DirectX *_DirectX;

			// デバイスロスト用
			std::list<LostResource*>::iterator _sIt;
		};

		//==============================================================================
		// データ
		//------------------------------------------------------------------------------
	}
}
#endif
//EOF