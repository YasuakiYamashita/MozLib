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
#include <map>

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
		class window
		{
		public:

			// �ʖ�
			typedef std::map<HWND, window*> ProcMap;

			// �R���X�g���N�^
			window(HINSTANCE hInstance, const char * windowName, const char * className = "mozWindow", unsigned int width = 1280, unsigned int height = 720);

			// �f�X�g���N�^
			virtual ~window();

			// ������Ă�
			int run(void);

			// �v���V�[�W��
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			// �n���h���擾
			inline HWND GetHWnd(void){ return _hWnd; }
			inline HINSTANCE GetHInstance(void){ return _hInstance; }

			// �T�C�Y�擾
			inline unsigned int GetWidth(void){ return _width; }
			inline unsigned int GetHeight(void){ return _height; }

			// �E�B���h�E�̖��O��ς���
			void SetWindowName(const char *);

		private:
			// �n���h��
			HINSTANCE	_hInstance;

			// hWnd
			HWND		_hWnd;

			// �T�C�Y
			unsigned int _width, _height;

			// windowClass
			const char* _className;

			// hWnd�ƃv���V�[�W���̑Ή��Â�
			static ProcMap _procMap;
			static window* _firstWindow;

			// �I���t���O
			bool _IsEnd;

		protected:
			// �v���V�[�W��
			virtual void _wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

			// Update
			virtual void Update(void){};
			virtual void Init(void){};
			virtual void Uninit(void){};

			// �I��������
			void Exit(void){ _IsEnd = true; }
		};
	}
}


#endif
//EOF