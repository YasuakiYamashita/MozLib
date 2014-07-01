//==============================================================================
//
// �}���`�X���b�h�p[mozThread.h]
// Author : Yasuaki Yamashita : 2014/06/26
//
//==============================================================================

#pragma once

#ifndef _MOZTHREAD_H_
#define _MOZTHREAD_H_

//------------------------------------------------------------------------------
// include
//------------------------------------------------------------------------------
#include <Windows.h>

namespace moz
{
	namespace thread
	{
		//==============================================================================
		// class
		//------------------------------------------------------------------------------
		class Thread
		{
		public:
			/// type
			typedef DWORD(WINAPI *ThreadFunc)(void*);

		public:
			Thread(ThreadFunc callback, bool isSuspend = false, void *param = nullptr);
			~Thread();

			// �X���b�h������ł��邩
			bool IsDead(void);
			// �X���b�h����~����܂ŃX���[�v����
			void JoinSleep(void);
			// �ꎞ��~
			void Stop(void);
			// �ĊJ
			void Start(void);

		private:
			HANDLE m_hThread;	// �X���b�h�n���h��
			DWORD m_threadId;	// �X���b�hID
			bool m_isSuspend;	// �T�X�y���h��Ԃ�
			bool m_isExit;		// �I���t���O
		};

	}
}
#endif
//EOF