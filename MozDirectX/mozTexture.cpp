//==============================================================================
//
// テクスチャ管理[mozTexture.cpp]
// Author : Yasuaki Yamashita : 2014/07/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include <mozCommon.h>
#include "mozDirectX.h"
#include <stdlib.h>
#include <windows.h>

namespace moz
{
	namespace DirectX
	{
		// ロードできるファイル一覧
		const char * cLoadFiles[] =
		{
			"bmp",
			"dds",
			"dib",
			"jpg",
			"png",
			"tga"
		};

		// ロードできるテクスチャの数
		const unsigned char cLoadMaxFile = sizeof(cLoadFiles) / sizeof(char *);

		//==============================================================================
		// テクスチャコンテナ　コンストラクタ
		//------------------------------------------------------------------------------
		TextureManager::_texContainer::_texContainer(const char* TexPath, const char* TexName, const TextureManager* TexManager, const char* GropName)
			: _sGropName(GropName)
			, _sTexName(TexName)
		{
			// テクスチャ作成
			D3DXCreateTextureFromFile(TexManager->_DirectX->GetDevice(), TexPath, &_tex);
		}

		//==============================================================================
		// テクスチャコンテナ　デストラクタ
		//------------------------------------------------------------------------------
		TextureManager::_texContainer::~_texContainer()
		{
			SAFE_RELEASE(_tex);
		}

		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		TextureManager::TextureManager(DirectX* _directX)
			: _DirectX(_directX)
		{
			// 一応クリア
			_Texlist.clear();

			// デバイスロストした時の登録
			_sIt = _DirectX->SetLostResource(this);
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		TextureManager::~TextureManager()
		{
			// 全てのテクスチャ削除
			for (auto it = _Texlist.begin(); it != _Texlist.end();)
			{
				delete it->second;
				it = _Texlist.erase(it);
			}

			// デバイスロストフック削除
			_DirectX->DelLostResource(_sIt);
		}

		//==============================================================================
		// テクスチャ一個ロード
		//------------------------------------------------------------------------------
		const TexContainer* TextureManager::LoadTex(const char * FilePath, const char * GropName)
		{
			std::string texname = FilePath;	// ファイル名
			unsigned int find = 0u;			// 文字探索用

			// エラーチェック
			if (FilePath == nullptr || GropName == nullptr)
			{
				return nullptr;
			}

			// テクスチャ名のみ取得
			find = texname.find_last_of('/') + 1;
			if (find != std::string::npos)
				texname.erase(0, find);
			find = texname.find_last_of('\\') + 1;
			if (find != std::string::npos)
				texname.erase(0, find);
			find = texname.find_first_of('.');
			if (find != std::string::npos)
				texname.erase(find, texname.size());

			// テクスチャ名がない場合
			if (texname.empty())
				return nullptr;

			// もうロードされていないかチェック
			auto it = _Texlist.find(texname);
			
			// ロードしてあったらそれを返す
			if (it != _Texlist.end())
			{
				return it->second;
			}

			// クリエイト
			_texContainer* tex = new _texContainer(FilePath, texname.c_str(), this, GropName);
			
			// テクスチャ取得失敗
			if (tex->GetTex() == nullptr)
			{
				SAFE_DELETE(tex);
			}

			// テクスチャ取得成功
			else
			{
				// mapに登録
				_Texlist.insert(texlist::value_type(texname, tex));
			}

			return tex;
		}

		//==============================================================================
		// ディレクトリを全部ロード
		//------------------------------------------------------------------------------
		void TextureManager::LoadDirectory(const char * Path, const char * GropName)
		{
			std::string sFilePass = Path;
			
			// 最後にスラッシュがない場合
			if (sFilePass[sFilePass.size() - 1] != '/' && sFilePass[sFilePass.size() - 1] != '\\')
			{
				sFilePass += "/";
			}

			// ディレクトリ用変数
			HANDLE hFind;
			WIN32_FIND_DATA fd;

			// エラーチェック
			ASSERT(INVALID_HANDLE_VALUE != (hFind = FindFirstFile((sFilePass + "*.*").c_str(), &fd)), "ディレクトリが開きませんでした");
			if (INVALID_HANDLE_VALUE == hFind)
				return ;

			// ディレクトリを全て回す
			for (; FindNextFile(hFind, &fd);)
			{
				LPDIRECT3DTEXTURE9 tex = nullptr;
				std::string filename = fd.cFileName;

				// 拡張子を見てロードできるか判断
				unsigned int find = filename.find_last_of('.');

				// 拡張子がない場合
				if (find == std::string::npos)
					continue;

				// とりあえずコピー
				std::string fileExtension = filename.c_str() + (find + 1);
				
				// ロードフラグ
				bool loadFlag = false;

				// 拡張子を調べる
				for (int i = 0; i < cLoadMaxFile; i++)
				{
					find = fileExtension.find(cLoadFiles[i]);
					if (find != std::string::npos)
					{
						loadFlag = true;
						break;
					}
				}

				// ロードフラグがあったら読み込む
				if (loadFlag)
				{
					LoadTex((sFilePass + filename).c_str(), GropName);
				}
			}
		}

		//==============================================================================
		// おんなじグループを削除
		//------------------------------------------------------------------------------
		void TextureManager::DeleteGrop(const char * Grop)
		{
			// nullだったら
			if (Grop == nullptr)
			{
				return ;
			}

			// 全て探索
			for (auto it = _Texlist.begin(); it != _Texlist.end();)
			{
				// おんなじグループだったら
				if (it->second->GetGropName() == Grop)
				{
					delete it->second;
					it = _Texlist.erase(it);
				}
				// それ以外は飛ばす
				else
				{
					++it;
				}
			}
		}

		//==============================================================================
		// テクスチャ取得
		//------------------------------------------------------------------------------
		TexContainer* TextureManager::GetTex(const char * name)
		{
			if (name == nullptr) return nullptr;

			auto it = _Texlist.find(name);

			// 有った場合
			if (it != _Texlist.end())
			{
				return it->second;
			}

			return nullptr;
		}
	}
}
//EOF