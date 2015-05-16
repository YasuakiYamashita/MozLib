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
#include "mozTexture.h"
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <string>

namespace moz
{
	namespace DirectX
	{
		// ロードできるファイル一覧
		static const char * cLoadFiles[] =
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

		// アライメントSIZE
		static const int ALIGNMENT_SIZE = 4;
	}
}

//==============================================================================
// テクスチャマネージャ親クラス
//==============================================================================
namespace moz
{
	namespace DirectX
	{

		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		_textureManager::_textureManager(DirectX* _directX)
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
		_textureManager::~_textureManager()
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
		// おんなじグループを削除
		//------------------------------------------------------------------------------
		void _textureManager::DeleteGrop(const char * Grop)
		{
			// nullだったら
			if (Grop == nullptr)
			{
				return;
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
		TexContainer* _textureManager::GetTex(const char * name)
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

//==============================================================================
// テクスチャコンテナ　デストラクタ
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// テクスチャコンテナ　デストラクタ
		//------------------------------------------------------------------------------
		TexContainer::~TexContainer()
		{
			SAFE_RELEASE(_tex);
		}
	}
}

//==============================================================================
// テクスチャマネージャ
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// テクスチャコンテナ　コンストラクタ
		//------------------------------------------------------------------------------
		TextureManager::NormalTexContainer::NormalTexContainer(const char* TexPath, const char* TexName, const TextureManager* TexManager, const char* GropName)
			: _texContainer(TexName, GropName)
		{
			// テクスチャ作成
			D3DXCreateTextureFromFile(TexManager->_DirectX->GetDevice(), TexPath, &_tex);
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
			NormalTexContainer* tex = new NormalTexContainer(FilePath, texname.c_str(), this, GropName);
			
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


	}
}

//==============================================================================
// テクスチャマネージャ フォントから生成
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		FontTexManager::FontTexManager(DirectX* _directX, const char * fontFile)
			: _textureManager(_directX)
		{
			// フォントがロードされない場合はデフォルトフォントになる
			if (fontFile != nullptr)
			{
				// フォントファミリ名を取得する
				m_FontName = Font::getName(fontFile);

				if (!m_FontName.empty())
				{
					// フォントロード
					LoadResourceFont(fontFile);
				}
			}
		}

		//==============================================================================
		// フォント登録
		//------------------------------------------------------------------------------
		void FontTexManager::LoadResourceFont(const char* const pFontFilePass)
		{
			FILE *fp = nullptr;

			// ファイルオープン
			fopen_s(&fp, pFontFilePass, "rb");
			ASSERT(fp != nullptr, "ファイルがロードできませんでした！");
			if (fp == nullptr) return;

			// ファイルサイズ取得
			fseek(fp, 0, SEEK_END);
			const unsigned int nFontFileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			// ファイルサイズチェック
			ASSERT(nFontFileSize != 0, "ファイルサイズが異常です!");
			if (nFontFileSize == 0)
			{
				fclose(fp);
				return;
			}

			// フォントデータ格納用のメモリ領域を確保
			char *pBuffer = new char[nFontFileSize];

			// フォントファイルを丸ごとメモリに読み込む
			fread(pBuffer, nFontFileSize, 1, fp);

			// ファイルクローズ
			fclose(fp);

			// フォントとして追加
			DWORD fontCount = 0;
			HANDLE pHandle = nullptr;
			pHandle = AddFontMemResourceEx(pBuffer, nFontFileSize, NULL, &fontCount);
			ASSERT(pHandle != nullptr, "フォントロードできませんでした");

			// メモリ解放
			SAFE_DELETE_ARRAY(pBuffer);

			// エラーチェック
			if (!pHandle) return;
		}

		//==============================================================================
		// テクスチャ作成
		//------------------------------------------------------------------------------
		void FontTexManager::CreateTex(const std::string& text, const char * GropName)
		{
			// 文字数だけ回す
			for (unsigned int fontNum = 0, i = 0; i<text.size(); i++, fontNum++)
			{
				const char *pStr = &(text[i]);

				// マルチバイトか調べてそうだったらずらす
				if (IsDBCSLeadByte(*pStr))
				{
					++i;
				}
			}
		}

		//==============================================================================
		// テクスチャ作成
		//------------------------------------------------------------------------------
		FontTexManager::_FontTexContainer::_FontTexContainer(const char *strs, const FontTexManager * const TexManager, const char *GropName)
			:_texContainer(strs, GropName)
		{

			LPDIRECT3DDEVICE9 device = TexManager->_DirectX->GetDevice();

			// フォント情報の取得
			BYTE* bmp = nullptr;
			GetFontInfo(&bmp, strs);

			// 空テクスチャの生成
			SAFE_RELEASE(FontTexContainer::_tex);

			// テクスチャ作成
			device->CreateTexture(
				(UINT)m_size.x,
				(UINT)m_size.y,
				1,
				0,
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				&(FontTexContainer::_tex),
				NULL
				);

			// テクスチャ情報の書き込み
			WriteFont(bmp);

			// bmp 削除
			SAFE_DELETE_ARRAY(bmp);

		}

		//==============================================================================
		// フォント情報取得
		//------------------------------------------------------------------------------
		void FontTexManager::_FontTexContainer::GetFontInfo(BYTE** bmp, const char* ch)
		{
			CONST MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
			TEXTMETRIC text_metric;
			GLYPHMETRICS glyph_metrics;

			// フォント情報
			3 font = {
				nFontSize,
				0,
				0,
				0,
				nWeight,
				FALSE,
				TRUE,
				FALSE,
				SHIFTJIS_CHARSET,
				OUT_TT_ONLY_PRECIS,
				CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY,
				FIXED_PITCH | FF_MODERN,
				0
			};

			memcpy(font.lfFaceName, m_family, strlen(m_family));

			HFONT h_font = CreateFontIndirectA(&font);

			// デバイスコンテキスト取得
			// フォントを持たせないとGetGlyphOutline関数はエラーとなる
			HDC hdc = GetDC(NULL);
			HFONT oldFont = (HFONT)SelectObject(hdc, h_font);

			// 文字コード取得
			// 2バイト文字のコードは[文字コード]*256 + [先導コード]です
			UINT code = 0;
			if (IsDBCSLeadByte(*ch))
			{
				code = (BYTE)ch[0] << 8 | (BYTE)ch[1];
			}
			else
			{
				code = ch[0];
			}

			// ビットマップ取得
			DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &glyph_metrics, 0, NULL, &mat);
			*bmp = new BYTE[size];
			GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &glyph_metrics, size, *bmp, &mat);

			GetTextMetrics(hdc, &text_metric);

			m_size.x = (float)glyph_metrics.gmBlackBoxX + (ALIGNMENT_SIZE - 1)) / ALIGNMENT_SIZE * ALIGNMENT_SIZE;
			m_size.y = (float)glyph_metrics.gmBlackBoxY;

			m_offset.x = (float)glyph_metrics.gmptGlyphOrigin.x;
			m_offset.y = (float)glyph_metrics.gmptGlyphOrigin.y;
			m_ascent   = (float)text_metric.tmAscent;

			// 次の位置指定
			m_nextPos.x = (float)glyph_metrics.gmCellIncX;
			m_nextPos.y = (float)glyph_metrics.gmCellIncY;

			// オブジェクトの開放
			SelectObject(hdc, oldFont);
			DeleteObject(h_font);
			ReleaseDC(NULL, hdc);
		}


		//==============================================================================
		// マルチバイトの文字数を数える
		//------------------------------------------------------------------------------
		int FontTexManager::MultiByteStringLength(const char* str)
		{
			// 全体のバイト数を取得
			int numByte = strlen(str);

			// マルチバイト数を数える
			unsigned int multiByteCnt = 0;
			for (int cnt = 0; cnt < numByte; ++cnt)
			{
				// if マルチバイト文字の先頭なら
				if (IsDBCSLeadByte(str[cnt]))
				{
					// 次の文字へ
					++cnt;
				}

				// マルバイトカウント数の増加
				++multiByteCnt;
			}

			return multiByteCnt;
		}

		//==============================================================================
		// フォーマットからレベルを取得する
		//------------------------------------------------------------------------------
		int FontTexManager::GetLevelFromFormat(const int format)
		{
			switch (format)
			{
			case GGO_GRAY2_BITMAP:
				return 5;
			case GGO_GRAY4_BITMAP:
				return 17;
			case GGO_GRAY8_BITMAP:
				return 65;
			default:
				return 1;
			}
		}
	}
}

//==============================================================================
// フォントファミリ名を取得する
//------------------------------------------------------------------------------
std::string  moz::DirectX::Font::getName(const char* fileName)
{
	FILE* fp = nullptr;
	Table head;
	std::string str = "";

	fp = fopen(fileName, "rb");
	if (fp == nullptr) return str;

	//==============================================================================
	// ヘッダの中からnameテーブルを探す
	fseek(fp, 12, SEEK_SET);
	while (fread(&head, sizeof(char), 16, fp) != 0)
	{
		head.length = swapEndianness<unsigned long int>(head.length);
		head.offset = swapEndianness<unsigned long int>(head.offset);
		if (head.d[0] == 'n' && head.d[1] == 'a' && head.d[2] == 'm' && head.d[3] == 'e') break;
		head.offset = 0;
	}
	if (head.offset == 0) return str;
	NameTable nTable;
	fseek(fp, head.offset, SEEK_SET);
	fread(&nTable, sizeof(NameTable), 1, fp);
	encode(nTable.d, sizeof(nTable.d));

	//==============================================================================
	// ネームレコードの中からフォントファミリー名を探す
	NameRecord nRecord;
	for (int i = 0; i < nTable.count; i++)
	{
		fread(&nRecord, sizeof(NameRecord), 1, fp);
		encode(nRecord.d, sizeof(nRecord.d));
		if (nRecord.platformID == 0x3 && (nRecord.languageID == 0x411 || nRecord.languageID == 0x409) && nRecord.nameID == 0x1)
			break;

		if (i == nTable.count - 1) return str;
	}

	//==============================================================================
	// フォントファミリー名取得
	fseek(fp, head.offset + nTable.offset + nRecord.offset, SEEK_SET);
	unsigned char Name[MAX_PATH] = { 0 };
	fread(Name, sizeof(char), nRecord.length, fp);
	Name[nRecord.length] = 0;
	encode(Name, nRecord.length);
	fclose(fp);

	//==============================================================================
	// UTF-16をsjisに変換
	int out_size = WideCharToMultiByte(CP_ACP, 0, (OLECHAR*)Name, -1, NULL, 0, NULL, NULL);
	char *tmp = new char[out_size];
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)Name, nRecord.length, tmp, out_size, NULL, NULL);
	str = tmp;
	delete tmp;

	return str;
}
//EOF