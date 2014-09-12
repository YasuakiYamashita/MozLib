//==============================================================================
//
// Window�N���X[mozWindow.h]
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

#ifdef _DEBUG
#pragma comment(lib, "lib/mozWindow_d.lib")
#else
#pragma comment(lib, "lib/mozWindow.lib")
#endif

//==============================================================================
//  
// Window�\���N���X
// ������p�������Ďg���ĂˁI
//
//==============================================================================
namespace moz
{
	namespace window
	{
		//==============================================================================
		// Window�\�����邾��
		//------------------------------------------------------------------------------
		class window
		{
		public:


			// �R���X�g���N�^
			window(HINSTANCE hInstance, const char * windowName, const char * className = "mozWindow", unsigned int width = 1280u, unsigned int height = 720u);

			// �f�X�g���N�^
			virtual ~window();

			// ������ĂԂƑ��肾��
			int run(void);

			// �n���h���擾
			inline HWND GetHWnd(void) const { return _hWnd; }
			inline HINSTANCE GetHInstance(void) const { return _hInstance; }

			// �T�C�Y�擾
			inline unsigned int GetWidth(void) const { return _width; }
			inline unsigned int GetHeight(void) const { return _height; }

			// �E�B���h�E�̖��O��ς���
			void SetWindowName(const char *);

			// �I��������
			void Exit(void){ _IsEnd = true; }

			// �A�N�e�B�u���ǂ���
			bool GetActive(void){ return m_bActive; }

			// �v���V�[�W��
			virtual void _wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

			// ���݂̃E�C���h�E���[�h
			bool GetWindowMode(void){ return _bWindowMode; }

			// �t���X�N���[�����[�h
			void FlipWindowMode(void);

		private:
			// �n���h��
			HINSTANCE	_hInstance;

			// hWnd
			HWND		_hWnd;

			// �T�C�Y
			unsigned int _width, _height;

			// windowClass
			const char* _className;

			// ��ʂ��A�N�e�B�u���ǂ���
			bool m_bActive;
			void SetActive(bool);

			// �I���t���O
			bool _IsEnd;

			// �t���X�N���[�����[�h
			bool _bWindowMode;

		protected:
			// Update
			virtual void Update(void){};
			virtual void Init(void){};
			virtual void Uninit(void){};
		};
	}
}


#endif
//EOF