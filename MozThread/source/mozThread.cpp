//==============================================================================
//
// マルチスレッド用[mozThread.cpp]
// Author : Yasuaki Yamashita : 2014/06/26
//
//==============================================================================

//------------------------------------------------------------------------------
// include
//------------------------------------------------------------------------------
#include "mozThread.h"

namespace moz
{
	namespace thread
	{
		//==============================================================================
		// これから呼ぶ
		//------------------------------------------------------------------------------
		unsigned int WINAPI ThreadFunc::Func(void * func)
		{
			int ret = ((funcData*)func)->func->_Func((funcData*)func);
			// 終了
			_endthreadex( ret );
			return ret;
		}


		//==============================================================================
		// コンストラクタ
		//------------------------------------------------------------------------------
		Thread::Thread(ThreadFunc * param, const bool isSuspend, void * data)
			: m_hThread(nullptr)
			, m_threadId(0)
			, m_isSuspend(isSuspend)
			, m_data(nullptr)
		{
			m_data = new funcData(param, data);


			//m_hThread = CreateThread(nullptr, 0, ThreadFunc::Func, m_data, m_isSuspend ? CREATE_SUSPENDED : 0, &m_threadId);
			m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc::Func, m_data, m_isSuspend ? CREATE_SUSPENDED : 0, &m_threadId);


			if (m_hThread == nullptr) {
				m_isSuspend = true;
			}
		}

		//==============================================================================
		// デストラクタ
		//------------------------------------------------------------------------------
		Thread::~Thread()
		{
			// 終了まで待機
			JoinSleep();

			// ハンドルの解放
			CloseHandle(m_hThread);
			m_hThread = nullptr;

			delete m_data;
		}

		//==============================================================================
		// スレッドの死を確認
		//------------------------------------------------------------------------------
		bool Thread::IsDead(void)
		{
			return WAIT_OBJECT_0 == WaitForSingleObject(m_hThread, 0);
		}

		//==============================================================================
		// スレッドが停止するまで待機
		//------------------------------------------------------------------------------
		void Thread::JoinSleep(void)
		{
			Exit();
			Sleep(1);
			WaitForSingleObject(m_hThread, INFINITE);
			Sleep(1);
		}
		
		//==============================================================================
		// スレッドを再開させる
		//------------------------------------------------------------------------------
		void Thread::Start(void)
		{
			if (!m_isSuspend)
			{
				return;
			}

			// -1の場合はエラー
			int ret = ResumeThread(m_hThread);
			(ret);

			m_isSuspend = false;
		}

		//==============================================================================
		// スレッドを停止する
		//------------------------------------------------------------------------------
		void Thread::Stop(void)
		{
			if (m_isSuspend)
			{
				return;
			}

			// -1の場合はエラー
			int ret = SuspendThread(m_hThread);
			(ret);

			m_isSuspend = true;
		}
	}
}

//EOF