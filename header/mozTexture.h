//==============================================================================
//
// �e�N�X�`���Ȃ�[mozTexture.h]
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
			// �e�N�X�`���擾
			const LPDIRECT3DTEXTURE9& GetTex(void){ return _tex; }

		protected:
			TexContainer() :_tex(nullptr){};
			virtual ~TexContainer();

			// �e�N�X�`������
			LPDIRECT3DTEXTURE9 _tex;
		};

		//==============================================================================
		// TextureManager �e�N���X
		//------------------------------------------------------------------------------
		class _textureManager : public LostResource
		{
		protected:
			// �e�N�X�`���R���e�i
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
			// �e�N�X�`�����X�g
			typedef std::map<std::string, _texContainer*> texlist;

			_textureManager(DirectX* _directX);
			virtual ~_textureManager();

			// �����O���[�v���폜
			virtual void DeleteGrop(const char * Grop);

			// �t�@�C���e�N�X�`���擾
			virtual TexContainer* GetTex(const char * name);

			// �e�N�X�`��
			void Backup(void){};

			// �e�N�X�`��
			void Recover(void){};

		protected:
			// �f�[�^���X�g
			texlist _Texlist;

			// DirectX�N���X
			DirectX *_DirectX;

			// �f�o�C�X���X�g�p
			std::list<LostResource*>::iterator _sIt;
		};

		//==============================================================================
		// TextureManager
		//------------------------------------------------------------------------------
		class TextureManager : public _textureManager
		{
		protected:
			// �e�N�X�`���R���e�i
			class NormalTexContainer : public _texContainer
			{
			public:
				NormalTexContainer(const char* TexPath, const char* TexName, const TextureManager* TexManager, const char* GropName = "general");
			};
		public:
			TextureManager(DirectX * _directX) :_textureManager(_directX){};

			// �e�N�X�`���쐬
			const TexContainer* LoadTex(const char * FilePath, const char * GropName = "general");

			// �e�N�X�`���f�B���N�g��
			void LoadDirectory(const char * Path, const char * GropName);

		};

		//==============================================================================
		// �f�[�^
		//------------------------------------------------------------------------------
		//==============================================================================
		// �t�H���g�p�e�N�X�`���R���e�i
		//------------------------------------------------------------------------------
		class FontTexContainer : public TexContainer
		{
		public:
		
			// �Q�b�^���������Ă���
			const D3DXVECTOR2& GetCharSize(){ return m_size; }
			const float GetAscent(){ return m_ascent; }
			const D3DXVECTOR2& GetOffset(){ return m_offset; }
			const D3DXVECTOR2& GetNextPos(){ return m_nextPos; }

		protected:
			// �����T�C�Y
			D3DXVECTOR2 m_size;
			// ��ɂ��炷
			float m_ascent;
			// �x�[�X���C��
			D3DXVECTOR2 m_offset;
			// ���̃t�H���g�ʒu
			D3DXVECTOR2 m_nextPos;

			
		};

		//==============================================================================
		// ttf�t�@�C������e�N�X�`���쐬
		//------------------------------------------------------------------------------
		class FontTexManager : public _textureManager
		{
		protected:
			//==============================================================================
			// �t�H���g�p�e�N�X�`���R���e�i
			//------------------------------------------------------------------------------
			class _FontTexContainer : public FontTexContainer, public _texContainer
			{
			public :
				_FontTexContainer(const char *strs, const FontTexManager * const TexManager, const char *GropName = "general");
			
			private:
				void GetFontInfo(BYTE** bmp, const char* ch);

			};

		public:
			// �R���X�g���N�^
			FontTexManager(DirectX* _directX, const char * fontFile = nullptr);
			// �f�X�g���N�^
			virtual ~FontTexManager() {};

			// �e�N�X�`���쐬
			void CreateTex(const std::string& text, const char * GropName = "general");

			// �t�H���g�o�^
			void LoadResourceFont(const char* pFontFilePass);

			// �}���`�o�C�g�̕������𐔂���
			int MultiByteStringLength(const char* str);

			// �t�H�[�}�b�g���烌�x�����擾����
			int GetLevelFromFormat(const int format);

			// �t�H���g�t�@�~�����擾
			const std::string& GetFontName(void){ return m_FontName; }

		private:
			// �t�H���g���擾
			std::string m_FontName;
		};

		//==============================================================================
		// TTF�t�@�C��������p
		//------------------------------------------------------------------------------
		namespace Font
		{
			//==============================================================================
			// ttf�t�@�C���w�b�_�p
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
			// �r�b�O�G���f�B�A���ƃ��g���G���f�B�A����ϊ�
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
			// UTF-16BE��UTF-16LE��ϊ�
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
			//�@�t�H���g���擾
			//------------------------------------------------------------------------------
			std::string getName(const char* fileName);
		}
	}
}
#endif
//EOF