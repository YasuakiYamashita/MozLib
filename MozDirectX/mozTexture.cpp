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
#include <stdlib.h>
#include <windows.h>

namespace moz
{
	namespace DirectX
	{
		// ���[�h�ł���t�@�C���ꗗ
		const char * cLoadFiles[] =
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

		//==============================================================================
		// �e�N�X�`���R���e�i�@�R���X�g���N�^
		//------------------------------------------------------------------------------
		TextureManager::_texContainer::_texContainer(const char* TexPath, const char* TexName, const TextureManager* TexManager, const char* GropName)
			: _sGropName(GropName)
			, _sTexName(TexName)
		{
			// �e�N�X�`���쐬
			D3DXCreateTextureFromFile(TexManager->_DirectX->GetDevice(), TexPath, &_tex);
		}

		//==============================================================================
		// �e�N�X�`���R���e�i�@�f�X�g���N�^
		//------------------------------------------------------------------------------
		TextureManager::_texContainer::~_texContainer()
		{
			SAFE_RELEASE(_tex);
		}

		//==============================================================================
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		TextureManager::TextureManager(DirectX* _directX)
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
		TextureManager::~TextureManager()
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
			_texContainer* tex = new _texContainer(FilePath, texname.c_str(), this, GropName);
			
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

		//==============================================================================
		// ����Ȃ��O���[�v���폜
		//------------------------------------------------------------------------------
		void TextureManager::DeleteGrop(const char * Grop)
		{
			// null��������
			if (Grop == nullptr)
			{
				return ;
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
		TexContainer* TextureManager::GetTex(const char * name)
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
//EOF