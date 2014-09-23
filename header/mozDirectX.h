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
// �O���錾
//------------------------------------------------------------------------------
class window;

namespace moz
{
	namespace DirectX
	{
		//==============================================================================
		// �f�o�C�X���X�g�p  ���N���X
		//------------------------------------------------------------------------------
		class LostResource
		{
		public:
			virtual void Backup(void) = 0;    // �o�b�N�A�b�v
			virtual void Recover(void) = 0;   // ����
		};

		//==============================================================================
		// DirectX
		//------------------------------------------------------------------------------
		class DirectX
		{
		public:

			// �R���X�g���N�^
			DirectX(moz::window::window* window, bool bWindowMode = true);
			// �f�X�g���N�^
			virtual ~DirectX();

			// �f�o�C�X�擾
			LPDIRECT3DDEVICE9 GetDevice(void){ return m_pDevice; }

			// �S��ʃ��[�h �� �E�C���h�E���[�h��؂�ւ���
			void FlipScreenMode(void);

			// ������
			// �����@�F�@�I���t���O�����Ă����|�C���^
			void run(bool* isExit);

			// �f�o�C�X���X�g�p
			std::list<LostResource*>::iterator SetLostResource(LostResource * const);
			void DelLostResource(std::list<LostResource*>::iterator const);

			// Window�擾
			const moz::window::window* GetWindow(void) const { return m_pWindow; }

		protected:
			// �I�[�o�[���C�h����Γǂݍ��ނ�[
			virtual void Init(void)  {};
			virtual void Update(void){};
			virtual void Draw(void)  {};
			virtual void Uninit(void){};

		private:
			// �f�o�C�X��������
			void DeviceTryBack(void);

			// Device�I�u�W�F�N�g(�`��ɕK�v)
			LPDIRECT3DDEVICE9		m_pDevice;

			// �X�N���[�����[�h
			bool m_bWindowMode;
			D3DPRESENT_PARAMETERS m_d3dppFull;
			D3DPRESENT_PARAMETERS m_d3dppWindow;
			D3DPRESENT_PARAMETERS *m_d3dpp;

			// ���̃f�B�X�v���C���[�h
			D3DDISPLAYMODE m_d3ddm;

			// DirectX�f�o�C�XLOST�t���O
			bool m_IsDeviceLost;

			// �t���O
			void _FlipScreenMode(void);

			// �t���X�N���[���ɕς���t���O
			bool m_fullChenge;

			// �f�o�C�X���X�g�p
			std::list<LostResource*> m_DevLost;

		protected:
			// Window�|�C���^
			moz::window::window* m_pWindow;
		};

		//==============================================================================
		// TexContainer
		//------------------------------------------------------------------------------
		class TexContainer
		{
		public:
			// �e�N�X�`���擾
			const LPDIRECT3DTEXTURE9& GetTex(void){ return _tex; }

		protected:
			TexContainer():_tex(nullptr){};
			virtual ~TexContainer(){};

			// �e�N�X�`������
			LPDIRECT3DTEXTURE9 _tex;
		};

		//==============================================================================
		// TextureManager
		//------------------------------------------------------------------------------
		class TextureManager : public LostResource
		{
		private:
			// �e�N�X�`���R���e�i
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
			// �e�N�X�`�����X�g
			typedef std::map<std::string, _texContainer*> texlist;

			TextureManager(DirectX* _directX);
			~TextureManager();

			// �e�N�X�`���쐬
			const TexContainer* LoadTex(const char * FilePath, const char * GropName = "general");

			// �e�N�X�`���f�B���N�g��
			void LoadDirectory(const char * Path, const char * GropName);

			// �����O���[�v���폜
			void DeleteGrop(const char * Grop);

			// �t�@�C���e�N�X�`���擾
			TexContainer* GetTex(const char * name);

			// �e�N�X�`��
			void Backup(void){};

			// �e�N�X�`��
			void Recover(void){};

		private:
			// �f�[�^���X�g
			texlist _Texlist;

			// DirectX�N���X
			DirectX *_DirectX;

			// �f�o�C�X���X�g�p
			std::list<LostResource*>::iterator _sIt;
		};

		//==============================================================================
		// �f�[�^
		//------------------------------------------------------------------------------
	}
}
#endif
//EOF