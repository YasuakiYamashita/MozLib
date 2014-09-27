//==============================================================================
//
// ���͊Ǘ�[mozInput.h]
// Author : Yasuaki Yamashita : 2014/09/25
//
//==============================================================================

#pragma once

#ifndef _MOZINPUT_H_
#define _MOZINPUT_H_

//�G���[�o�͖h�~�p
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include <mozWindow.h>


#ifdef _DEBUG
#pragma comment(lib, "lib/mozInput_d.lib")
#else
#pragma comment(lib, "lib/mozInput.lib")
#endif


namespace moz
{
	namespace input
	{
		class InputKey;

		//==============================================================================
		// ���͊Ǘ��N���X
		//------------------------------------------------------------------------------
		class InputManager
		{
		public:
			InputManager(moz::window::window*);
			~InputManager();

			// �X�V����
			void Update(void);

			// �f�o�C�X
			const LPDIRECTINPUT8& GetDevice(void){ return m_pDInput; }
			const moz::window::window* GetWindow(void){ return m_window; }

		private:
			// �f�o�C�X
			LPDIRECTINPUT8	m_pDInput;
			
			// Window
			moz::window::window *m_window;
			InputKey* m_inputkey;
		};

		//==============================================================================
		// �f�o�C�X�p���N���X
		//------------------------------------------------------------------------------
		class InputDevice
		{
		public:
			InputDevice(InputManager* p) : m_pInputManager(p){};
			virtual ~InputDevice(){};
			virtual void Update(void) = 0;

			virtual bool GetPress(int nKey) = 0;
			virtual bool GetTrigger(int nKey) = 0;
			virtual bool GetRepeat(int nKey, int interval = 10) = 0;
			virtual bool GetRelease(int nKey) = 0;

		protected:
			InputManager* m_pInputManager;
			LPDIRECTINPUTDEVICE8 m_pDev;	//�f�o�C�X�ւ̃|�C���^
		};

		//==============================================================================
		// �L�[�{�[�h�p�f�o�C�X
		//------------------------------------------------------------------------------
		class InputKey : public InputDevice
		{
		public:
			InputKey(InputManager* p);
			~InputKey();
			void Update(void);

			bool GetPress(int nKey);
			bool GetTrigger(int nKey);
			bool GetRepeat(int nKey, int interval);
			bool GetRelease(int nKey);

		private:
			BYTE	m_aKeyState[256];
			BYTE	m_aKeyTrigger[256];
			BYTE	m_aKeyRelease[256];
			BYTE	m_aKeyRepeat[256];
		};

	}
}



#endif
//EOF