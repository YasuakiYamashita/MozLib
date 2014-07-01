//==============================================================================
//
// マルチスレッド用[mozThread.h]
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

			// スレッドが死んでいるか
			bool IsDead(void);
			// スレッドが停止するまでスリープする
			void JoinSleep(void);
			// 一時停止
			void Stop(void);
			// 再開
			void Start(void);

		private:
			HANDLE m_hThread;	// スレッドハンドル
			DWORD m_threadId;	// スレッドID
			bool m_isSuspend;	// サスペンド状態か
			bool m_isExit;		// 終了フラグ
		};

	}
}
#endif
//EOF