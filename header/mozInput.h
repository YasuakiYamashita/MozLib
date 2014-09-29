//==============================================================================
//
// 入力管理[mozInput.h]
// Author : Yasuaki Yamashita : 2014/09/25
//
//==============================================================================

#pragma once

#ifndef _MOZINPUT_H_
#define _MOZINPUT_H_

//エラー出力防止用
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
		// 入力管理クラス
		//------------------------------------------------------------------------------
		class InputManager
		{
		public:
			InputManager(moz::window::window*);
			~InputManager();

			// 更新処理
			void Update(void);

			// デバイス
			const LPDIRECTINPUT8& GetDevice(void){ return m_pDInput; }
			const moz::window::window* GetWindow(void){ return m_window; }

			 InputKey* const GetKey(void) const { return m_inputkey; }

		private:
			// デバイス
			LPDIRECTINPUT8	m_pDInput;
			
			// Window
			moz::window::window *m_window;
			InputKey* m_inputkey;
		};

		//==============================================================================
		// デバイス用基底クラス
		//------------------------------------------------------------------------------
		class InputDevice
		{
		public:
			InputDevice(InputManager* p) : m_pInputManager(p){};
			virtual ~InputDevice(){};
			virtual void Update(void) = 0;

			inline virtual bool GetPress(int nKey) = 0;
			inline virtual bool GetTrigger(int nKey) = 0;
			inline virtual bool GetRepeat(int nKey, int interval = 10) = 0;
			inline virtual bool GetRelease(int nKey) = 0;

		protected:
			InputManager* m_pInputManager;
			LPDIRECTINPUTDEVICE8 m_pDev;	//デバイスへのポインタ
		};

		//==============================================================================
		// キーボード用デバイス
		//------------------------------------------------------------------------------
		class InputKey : public InputDevice
		{
		public:
			InputKey(InputManager* p);
			~InputKey();
			void Update(void);

			inline bool GetPress(int nKey);
			inline bool GetTrigger(int nKey);
			inline bool GetRepeat(int nKey, int interval);
			inline bool GetRelease(int nKey);

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