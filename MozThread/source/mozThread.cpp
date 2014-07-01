//==============================================================================
//
// �}���`�X���b�h�p[mozThread.cpp]
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
		// �R���X�g���N�^
		//------------------------------------------------------------------------------
		Thread::Thread(ThreadFunc const callback, const bool isSuspend, void * param)
			: m_hThread(nullptr)
			, m_threadId(0)
			, m_isSuspend(isSuspend)
			, m_isExit(false)
		{
			m_hThread = CreateThread(nullptr, 0, callback, param, m_isSuspend ? CREATE_SUSPENDED : 0, &m_threadId);

			if (m_hThread == nullptr) {
				m_isExit = true;
				m_isSuspend = true;
			}
		}

		//==============================================================================
		// �f�X�g���N�^
		//------------------------------------------------------------------------------
		Thread::~Thread()
		{
			// �I���܂őҋ@
			JoinSleep();

			// �n���h���̉��
			CloseHandle(m_hThread);
			m_hThread = nullptr;
		}

		//==============================================================================
		// �X���b�h�̎����m�F
		//------------------------------------------------------------------------------
		bool Thread::IsDead(void)
		{
			return WAIT_OBJECT_0 == WaitForSingleObject(m_hThread, 0);
		}

		//==============================================================================
		// �X���b�h����~����܂őҋ@
		//------------------------------------------------------------------------------
		void Thread::JoinSleep(void)
		{
			WaitForSingleObject(m_hThread, INFINITE);
			Sleep(1);
		}
		
		//==============================================================================
		// �X���b�h���ĊJ������
		//------------------------------------------------------------------------------
		void Thread::Start(void)
		{
			if (!m_isSuspend)
			{
				return;
			}

			// -1�̏ꍇ�̓G���[
			int ret = ResumeThread(m_hThread);
			(ret);

			m_isSuspend = false;
		}

		//==============================================================================
		// �X���b�h���~����
		//------------------------------------------------------------------------------
		void Thread::Stop(void)
		{
			if (m_isSuspend)
			{
				return;
			}

			// -1�̏ꍇ�̓G���[
			int ret = SuspendThread(m_hThread);
			(ret);

			m_isSuspend = true;
		}
	}
}

//EOF