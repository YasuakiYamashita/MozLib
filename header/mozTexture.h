//==============================================================================
//
// テクスチャなど[mozTexture.h]
// Author : Yasuaki Yamashita : 2014/10/13
//
//==============================================================================

#pragma once

#ifndef _MOZTEXTURE_H_
#define _MOZTEXTURE_H_

//==============================================================================
// 
//------------------------------------------------------------------------------
namespace moz
{
	namespace DirectX
	{
		static const int nFontSize = 32;
		static const int nWeight = 500;

		//==============================================================================
		// TexContainer
		//------------------------------------------------------------------------------
		class TexContainer
		{
		public:
			// テクスチャ取得
			const LPDIRECT3DTEXTURE9& GetTex(void){ return _tex; }

		protected:
			TexContainer() :_tex(nullptr){};
			virtual ~TexContainer();

			// テクスチャ実態
			LPDIRECT3DTEXTURE9 _tex;
		};

		//==============================================================================
		// TextureManager 親クラス
		//------------------------------------------------------------------------------
		class _textureManager : public LostResource
		{
		protected:
			// テクスチャコンテナ
			class _texContainer : public TexContainer
			{
			public:
				_texContainer(const char *TexName, const char *GropName) :_sTexName(TexName), _sGropName(GropName){};

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

			_textureManager(DirectX* _directX);
			virtual ~_textureManager();

			// 同じグループを削除
			virtual void DeleteGrop(const char * Grop);

			// ファイルテクスチャ取得
			virtual TexContainer* GetTex(const char * name);

			// テクスチャ
			void Backup(void){};

			// テクスチャ
			void Recover(void){};

		protected:
			// データリスト
			texlist _Texlist;

			// DirectXクラス
			DirectX *_DirectX;

			// デバイスロスト用
			std::list<LostResource*>::iterator _sIt;
		};

		//==============================================================================
		// TextureManager
		//------------------------------------------------------------------------------
		class TextureManager : public _textureManager
		{
		protected:
			// テクスチャコンテナ
			class NormalTexContainer : public _texContainer
			{
			public:
				NormalTexContainer(const char* TexPath, const char* TexName, const TextureManager* TexManager, const char* GropName = "general");
			};
		public:
			TextureManager(DirectX * _directX) :_textureManager(_directX){};

			// テクスチャ作成
			const TexContainer* LoadTex(const char * FilePath, const char * GropName = "general");

			// テクスチャディレクトリ
			void LoadDirectory(const char * Path, const char * GropName);

		};

		//==============================================================================
		// データ
		//------------------------------------------------------------------------------
		//==============================================================================
		// フォント用テクスチャコンテナ
		//------------------------------------------------------------------------------
		class FontTexContainer : public TexContainer
		{
		public:
		
			// ゲッタだけ書いておく
			const D3DXVECTOR2& GetCharSize(){ return m_size; }
			const float GetAscent(){ return m_ascent; }
			const D3DXVECTOR2& GetOffset(){ return m_offset; }
			const D3DXVECTOR2& GetNextPos(){ return m_nextPos; }

		protected:
			// 文字サイズ
			D3DXVECTOR2 m_size;
			// 上にずらす
			float m_ascent;
			// ベースライン
			D3DXVECTOR2 m_offset;
			// 次のフォント位置
			D3DXVECTOR2 m_nextPos;

			
		};

		//==============================================================================
		// ttfファイルからテクスチャ作成
		//------------------------------------------------------------------------------
		class FontTexManager : public _textureManager
		{
		protected:
			//==============================================================================
			// フォント用テクスチャコンテナ
			//------------------------------------------------------------------------------
			class _FontTexContainer : public FontTexContainer, public _texContainer
			{
			public :
				_FontTexContainer(const char *strs, const FontTexManager * const TexManager, const char *GropName = "general");
			
			private:
				void GetFontInfo(BYTE** bmp, const char* ch);

			};

		public:
			// コンストラクタ
			FontTexManager(DirectX* _directX, const char * fontFile = nullptr);
			// デストラクタ
			virtual ~FontTexManager() {};

			// テクスチャ作成
			void CreateTex(const std::string& text, const char * GropName = "general");

			// フォント登録
			void LoadResourceFont(const char* pFontFilePass);

			// マルチバイトの文字数を数える
			int MultiByteStringLength(const char* str);

			// フォーマットからレベルを取得する
			int GetLevelFromFormat(const int format);

			// フォントファミリ名取得
			const std::string& GetFontName(void){ return m_FontName; }

		private:
			// フォント名取得
			std::string m_FontName;
		};

		//==============================================================================
		// TTFファイルいじる用
		//------------------------------------------------------------------------------
		namespace Font
		{
			//==============================================================================
			// ttfファイルヘッダ用
			//-------------------------------------------------------------------------------
			union Table
			{
				struct
				{
					unsigned char tag[4];
					unsigned char checkSum[4];
					unsigned long offset;
					unsigned long length;
				};

				unsigned char d[16];
			};

			union NameTable
			{
				struct {
					unsigned short format;
					unsigned short count;
					unsigned short offset;
				};
				unsigned char d[6];
			};

			union NameRecord
			{
				struct {
					unsigned short platformID;
					unsigned short encodingID;
					unsigned short languageID;
					unsigned short nameID;
					unsigned short length;
					unsigned short offset;
				};

				unsigned char d[12];
			};

			//==============================================================================
			// ビッグエンディアンとリトルエンディアンを変換
			//------------------------------------------------------------------------------
			template <typename T>
			T swapEndianness(const T& t)
			{
				unsigned char byteCount = sizeof(T);
				T byteMask = 0xFF;
				T newVal = 0;

				for (unsigned char i = 0; i < byteCount; i++)
				{
					T mask = (byteMask << (i * 8));
					T val = ((t&mask) >> i * 8);
					unsigned char offset = (byteCount - i - 1) * 8;
					T destMask = byteMask << offset;

					newVal |= (((val) << offset) & destMask);
				}

				return newVal;
			}

			//==============================================================================
			// UTF-16BEとUTF-16LEを変換
			//------------------------------------------------------------------------------
			void encode(unsigned char* str, unsigned int size)
			{
				for (unsigned int i = 0; i < size; i += 2)
				{
					unsigned char tmp = str[i];
					str[i] = str[i + 1];
					str[i + 1] = tmp;
				}
			}

			//==============================================================================
			//　フォント名取得
			//------------------------------------------------------------------------------
			std::string getName(const char* fileName);
		}
	}
}
#endif
//EOF