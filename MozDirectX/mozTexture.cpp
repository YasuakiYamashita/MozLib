//==============================================================================
//
// �e�N�X�`���Ǘ�[mozTexture.cpp]
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
		// ���[�h�ł���t�@�C���ꗗ
		static const char * cLoadFiles[] =
		{
			"bmp",
			"dds",
			"dib",
			"jpg",
			"png",
			"tga"
		};

		// ���[�h�ł���e�N�X�`���̐�
		const unsigned char cLoadMaxFile = sizeof(cLoadFiles) / sizeof(char *);

		// �A���C�����gSIZE
		static const int ALIGNMENT_SIZE = 4;
	}
}

//==============================================================================
// �e�N�X�`���}�l�[�W���e�N���X
//==============================================================================
namespace moz
{
	namespace DirectX
	{

		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		_textureManager::_textureManager(DirectX* _directX)
			: _DirectX(_directX)
		{
			// �ꉞ�N���A
			_Texlist.clear();

			// �f�o�C�X���X�g�������̓o�^
			_sIt = _DirectX->SetLostResource(this);
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		_textureManager::~_textureManager()
		{
			// �S�Ẵe�N�X�`���폜
			for (auto it = _Texlist.begin(); it != _Texlist.end();)
			{
				delete it->second;
				it = _Texlist.erase(it);
			}

			// �f�o�C�X���X�g�t�b�N�폜
			_DirectX->DelLostResource(_sIt);
		}
		//==============================================================================
		// ����Ȃ��O���[�v���폜
		//------------------------------------------------------------------------------
		void _textureManager::DeleteGrop(const char * Grop)
		{
			// null��������
			if (Grop == nullptr)
			{
				return;
			}

			// �S�ĒT��
			for (auto it = _Texlist.begin(); it != _Texlist.end();)
			{
				// ����Ȃ��O���[�v��������
				if (it->second->GetGropName() == Grop)
				{
					delete it->second;
					it = _Texlist.erase(it);
				}
				// ����ȊO�͔�΂�
				else
				{
					++it;
				}
			}
		}

		//==============================================================================
		// �e�N�X�`���擾
		//------------------------------------------------------------------------------
		TexContainer* _textureManager::GetTex(const char * name)
		{
			if (name == nullptr) return nullptr;

			auto it = _Texlist.find(name);

			// �L�����ꍇ
			if (it != _Texlist.end())
			{
				return it->second;
			}

			return nullptr;
		}
	}
}

//==============================================================================
// �e�N�X�`���R���e�i�@�f�X�g���N�^
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �e�N�X�`���R���e�i�@�f�X�g���N�^
		//------------------------------------------------------------------------------
		TexContainer::~TexContainer()
		{
			SAFE_RELEASE(_tex);
		}
	}
}

//==============================================================================
// �e�N�X�`���}�l�[�W��
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �e�N�X�`���R���e�i�@�R���X�g���N�^
		//------------------------------------------------------------------------------
		TextureManager::NormalTexContainer::NormalTexContainer(const char* TexPath, const char* TexName, const TextureManager* TexManager, const char* GropName)
			: _texContainer(TexName, GropName)
		{
			// �e�N�X�`���쐬
			D3DXCreateTextureFromFile(TexManager->_DirectX->GetDevice(), TexPath, &_tex);
		}

		//==============================================================================
		// �e�N�X�`������[�h
		//------------------------------------------------------------------------------
		const TexContainer* TextureManager::LoadTex(const char * FilePath, const char * GropName)
		{
			std::string texname = FilePath;	// �t�@�C����
			unsigned int find = 0u;			// �����T���p

			// �G���[�`�F�b�N
			if (FilePath == nullptr || GropName == nullptr)
			{
				return nullptr;
			}

			// �e�N�X�`�����̂ݎ擾
			find = texname.find_last_of('/') + 1;
			if (find != std::string::npos)
				texname.erase(0, find);
			find = texname.find_last_of('\\') + 1;
			if (find != std::string::npos)
				texname.erase(0, find);
			find = texname.find_first_of('.');
			if (find != std::string::npos)
				texname.erase(find, texname.size());

			// �e�N�X�`�������Ȃ��ꍇ
			if (texname.empty())
				return nullptr;

			// �������[�h����Ă��Ȃ����`�F�b�N
			auto it = _Texlist.find(texname);
			
			// ���[�h���Ă������炻���Ԃ�
			if (it != _Texlist.end())
			{
				return it->second;
			}

			// �N���G�C�g
			NormalTexContainer* tex = new NormalTexContainer(FilePath, texname.c_str(), this, GropName);
			
			// �e�N�X�`���擾���s
			if (tex->GetTex() == nullptr)
			{
				SAFE_DELETE(tex);
			}

			// �e�N�X�`���擾����
			else
			{
				// map�ɓo�^
				_Texlist.insert(texlist::value_type(texname, tex));
			}

			return tex;
		}

		//==============================================================================
		// �f�B���N�g����S�����[�h
		//------------------------------------------------------------------------------
		void TextureManager::LoadDirectory(const char * Path, const char * GropName)
		{
			std::string sFilePass = Path;
			
			// �Ō�ɃX���b�V�����Ȃ��ꍇ
			if (sFilePass[sFilePass.size() - 1] != '/' && sFilePass[sFilePass.size() - 1] != '\\')
			{
				sFilePass += "/";
			}

			// �f�B���N�g���p�ϐ�
			HANDLE hFind;
			WIN32_FIND_DATA fd;

			// �G���[�`�F�b�N
			ASSERT(INVALID_HANDLE_VALUE != (hFind = FindFirstFile((sFilePass + "*.*").c_str(), &fd)), "�f�B���N�g�����J���܂���ł���");
			if (INVALID_HANDLE_VALUE == hFind)
				return ;

			// �f�B���N�g����S�ĉ�
			for (; FindNextFile(hFind, &fd);)
			{
				LPDIRECT3DTEXTURE9 tex = nullptr;
				std::string filename = fd.cFileName;

				// �g���q�����ă��[�h�ł��邩���f
				unsigned int find = filename.find_last_of('.');

				// �g���q���Ȃ��ꍇ
				if (find == std::string::npos)
					continue;

				// �Ƃ肠�����R�s�[
				std::string fileExtension = filename.c_str() + (find + 1);
				
				// ���[�h�t���O
				bool loadFlag = false;

				// �g���q�𒲂ׂ�
				for (int i = 0; i < cLoadMaxFile; i++)
				{
					find = fileExtension.find(cLoadFiles[i]);
					if (find != std::string::npos)
					{
						loadFlag = true;
						break;
					}
				}

				// ���[�h�t���O����������ǂݍ���
				if (loadFlag)
				{
					LoadTex((sFilePass + filename).c_str(), GropName);
				}
			}
		}


	}
}

//==============================================================================
// �e�N�X�`���}�l�[�W�� �t�H���g���琶��
//==============================================================================
namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		FontTexManager::FontTexManager(DirectX* _directX, const char * fontFile)
			: _textureManager(_directX)
		{
			// �t�H���g�����[�h����Ȃ��ꍇ�̓f�t�H���g�t�H���g�ɂȂ�
			if (fontFile != nullptr)
			{
				// �t�H���g�t�@�~�������擾����
				m_FontName = Font::getName(fontFile);

				if (!m_FontName.empty())
				{
					// �t�H���g���[�h
					LoadResourceFont(fontFile);
				}
			}
		}

		//==============================================================================
		// �t�H���g�o�^
		//------------------------------------------------------------------------------
		void FontTexManager::LoadResourceFont(const char* const pFontFilePass)
		{
			FILE *fp = nullptr;

			// �t�@�C���I�[�v��
			fopen_s(&fp, pFontFilePass, "rb");
			ASSERT(fp != nullptr, "�t�@�C�������[�h�ł��܂���ł����I");
			if (fp == nullptr) return;

			// �t�@�C���T�C�Y�擾
			fseek(fp, 0, SEEK_END);
			const unsigned int nFontFileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			// �t�@�C���T�C�Y�`�F�b�N
			ASSERT(nFontFileSize != 0, "�t�@�C���T�C�Y���ُ�ł�!");
			if (nFontFileSize == 0)
			{
				fclose(fp);
				return;
			}

			// �t�H���g�f�[�^�i�[�p�̃������̈���m��
			char *pBuffer = new char[nFontFileSize];

			// �t�H���g�t�@�C�����ۂ��ƃ������ɓǂݍ���
			fread(pBuffer, nFontFileSize, 1, fp);

			// �t�@�C���N���[�Y
			fclose(fp);

			// �t�H���g�Ƃ��Ēǉ�
			DWORD fontCount = 0;
			HANDLE pHandle = nullptr;
			pHandle = AddFontMemResourceEx(pBuffer, nFontFileSize, NULL, &fontCount);
			ASSERT(pHandle != nullptr, "�t�H���g���[�h�ł��܂���ł���");

			// ���������
			SAFE_DELETE_ARRAY(pBuffer);

			// �G���[�`�F�b�N
			if (!pHandle) return;
		}

		//==============================================================================
		// �e�N�X�`���쐬
		//------------------------------------------------------------------------------
		void FontTexManager::CreateTex(const std::string& text, const char * GropName)
		{
			// ������������
			for (unsigned int fontNum = 0, i = 0; i<text.size(); i++, fontNum++)
			{
				const char *pStr = &(text[i]);

				// �}���`�o�C�g�����ׂĂ����������炸�炷
				if (IsDBCSLeadByte(*pStr))
				{
					++i;
				}
			}
		}

		//==============================================================================
		// �e�N�X�`���쐬
		//------------------------------------------------------------------------------
		FontTexManager::_FontTexContainer::_FontTexContainer(const char *strs, const FontTexManager * const TexManager, const char *GropName)
			:_texContainer(strs, GropName)
		{

			LPDIRECT3DDEVICE9 device = TexManager->_DirectX->GetDevice();

			// �t�H���g���̎擾
			BYTE* bmp = nullptr;
			GetFontInfo(&bmp, strs);

			// ��e�N�X�`���̐���
			SAFE_RELEASE(FontTexContainer::_tex);

			// �e�N�X�`���쐬
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

			// �e�N�X�`�����̏�������
			WriteFont(bmp);

			// bmp �폜
			SAFE_DELETE_ARRAY(bmp);

		}

		//==============================================================================
		// �t�H���g���擾
		//------------------------------------------------------------------------------
		void FontTexManager::_FontTexContainer::GetFontInfo(BYTE** bmp, const char* ch)
		{
			CONST MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
			TEXTMETRIC text_metric;
			GLYPHMETRICS glyph_metrics;

			// �t�H���g���
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

			// �f�o�C�X�R���e�L�X�g�擾
			// �t�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
			HDC hdc = GetDC(NULL);
			HFONT oldFont = (HFONT)SelectObject(hdc, h_font);

			// �����R�[�h�擾
			// 2�o�C�g�����̃R�[�h��[�����R�[�h]*256 + [�擱�R�[�h]�ł�
			UINT code = 0;
			if (IsDBCSLeadByte(*ch))
			{
				code = (BYTE)ch[0] << 8 | (BYTE)ch[1];
			}
			else
			{
				code = ch[0];
			}

			// �r�b�g�}�b�v�擾
			DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &glyph_metrics, 0, NULL, &mat);
			*bmp = new BYTE[size];
			GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &glyph_metrics, size, *bmp, &mat);

			GetTextMetrics(hdc, &text_metric);

			m_size.x = (float)glyph_metrics.gmBlackBoxX + (ALIGNMENT_SIZE - 1)) / ALIGNMENT_SIZE * ALIGNMENT_SIZE;
			m_size.y = (float)glyph_metrics.gmBlackBoxY;

			m_offset.x = (float)glyph_metrics.gmptGlyphOrigin.x;
			m_offset.y = (float)glyph_metrics.gmptGlyphOrigin.y;
			m_ascent   = (float)text_metric.tmAscent;

			// ���̈ʒu�w��
			m_nextPos.x = (float)glyph_metrics.gmCellIncX;
			m_nextPos.y = (float)glyph_metrics.gmCellIncY;

			// �I�u�W�F�N�g�̊J��
			SelectObject(hdc, oldFont);
			DeleteObject(h_font);
			ReleaseDC(NULL, hdc);
		}


		//==============================================================================
		// �}���`�o�C�g�̕������𐔂���
		//------------------------------------------------------------------------------
		int FontTexManager::MultiByteStringLength(const char* str)
		{
			// �S�̂̃o�C�g�����擾
			int numByte = strlen(str);

			// �}���`�o�C�g���𐔂���
			unsigned int multiByteCnt = 0;
			for (int cnt = 0; cnt < numByte; ++cnt)
			{
				// if �}���`�o�C�g�����̐擪�Ȃ�
				if (IsDBCSLeadByte(str[cnt]))
				{
					// ���̕�����
					++cnt;
				}

				// �}���o�C�g�J�E���g���̑���
				++multiByteCnt;
			}

			return multiByteCnt;
		}

		//==============================================================================
		// �t�H�[�}�b�g���烌�x�����擾����
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
// �t�H���g�t�@�~�������擾����
//------------------------------------------------------------------------------
std::string  moz::DirectX::Font::getName(const char* fileName)
{
	FILE* fp = nullptr;
	Table head;
	std::string str = "";

	fp = fopen(fileName, "rb");
	if (fp == nullptr) return str;

	//==============================================================================
	// �w�b�_�̒�����name�e�[�u����T��
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
	// �l�[�����R�[�h�̒�����t�H���g�t�@�~���[����T��
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
	// �t�H���g�t�@�~���[���擾
	fseek(fp, head.offset + nTable.offset + nRecord.offset, SEEK_SET);
	unsigned char Name[MAX_PATH] = { 0 };
	fread(Name, sizeof(char), nRecord.length, fp);
	Name[nRecord.length] = 0;
	encode(Name, nRecord.length);
	fclose(fp);

	//==============================================================================
	// UTF-16��sjis�ɕϊ�
	int out_size = WideCharToMultiByte(CP_ACP, 0, (OLECHAR*)Name, -1, NULL, 0, NULL, NULL);
	char *tmp = new char[out_size];
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)Name, nRecord.length, tmp, out_size, NULL, NULL);
	str = tmp;
	delete tmp;

	return str;
}
//EOF