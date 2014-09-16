//==============================================================================
//
// �}���`�X���b�h�p[mozThread.h]
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
		// ���M�f�[�^
		//------------------------------------------------------------------------------
		struct funcData
		{
			funcData(ThreadFunc* _func, void* _data = nullptr) :data(_data), func(_func), isExit(false){};
			void * data;
			ThreadFunc* func;
			bool isExit;
		};

		//==============================================================================
		// ������p�������Ďg���Ă�
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

			// �X���b�h������ł��邩
			bool IsDead(void);
			// �X���b�h����~����܂ŃX���[�v����
			void JoinSleep(void);
			// �ꎞ��~
			void Stop(void);
			// �ĊJ
			void Start(void);

			// �I�����ǂ���
			bool IsExit(void){ return m_data->isExit; }

			// �I��������
			void Exit(void){ m_data->isExit = true; }
		private:
			HANDLE			m_hThread;		// �X���b�h�n���h��
			unsigned int	m_threadId;		// �X���b�hID
			bool			m_isSuspend;		// �T�X�y���h��Ԃ�
			funcData*		m_data;
		};

	}
}
#endif
//EOF