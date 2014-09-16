//==============================================================================
//
// マルチスレッド用[mozThread.h]
// Author : Yasuaki Yamashita : 2014/06/26
//
//==============================================================================

#pragma once

#ifndef _MOZTHREAD_H_
#define _MOZTHREAD_H_

#ifdef _DEBUG
#pragma comment(lib, "lib/mozThread_d.lib")
#else
#pragma comment(lib, "lib/mozThread.lib")
#endif


//------------------------------------------------------------------------------
// include
//------------------------------------------------------------------------------
#include <Windows.h>
#include <process.h>

namespace moz
{
	namespace thread
	{
		class ThreadFunc;

		//==============================================================================
		// 送信データ
		//------------------------------------------------------------------------------
		struct funcData
		{
			funcData(ThreadFunc* _func, void* _data = nullptr) :data(_data), func(_func), isExit(false){};
			void * data;
			ThreadFunc* func;
			bool isExit;
		};

		//==============================================================================
		// これを継承させて使ってね
		//------------------------------------------------------------------------------
		class ThreadFunc
		{
		public:
			static unsigned int WINAPI Func(void *);
	
		private:
			virtual DWORD _Func(funcData *) = 0;
		};

		//==============================================================================
		// class
		//------------------------------------------------------------------------------
		class Thread
		{
		public:
			Thread(ThreadFunc *param, bool isSuspend = false, void* data = nullptr);
			~Thread();

			// スレッドが死んでいるか
			bool IsDead(void);
			// スレッドが停止するまでスリープする
			void JoinSleep(void);
			// 一時停止
			void Stop(void);
			// 再開
			void Start(void);

			// 終了かどうか
			bool IsExit(void){ return m_data->isExit; }

			// 終了させる
			void Exit(void){ m_data->isExit = true; }
		private:
			HANDLE			m_hThread;		// スレッドハンドル
			unsigned int	m_threadId;		// スレッドID
			bool			m_isSuspend;		// サスペンド状態か
			funcData*		m_data;
		};

	}
}
#endif
//EOF