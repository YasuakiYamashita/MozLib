//==============================================================================
//
// �A�T�[�g�p[mozAssert.cpp]
// Author : Yasuaki Yamashita : 2014/07/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "mozAssert.h"
#include <stdio.h>
#include <Windows.h>

//------------------------------------------------------------------------------
// �A�T�[�g
//------------------------------------------------------------------------------
void MyAssert(char *pExp, char *pFileName, int nLine, char *pFormat, ...)
{
	const int STRSIZE = 128;
	char	aStr[STRSIZE];		// �\��������
	char aStrResult[512];			// �ŏI���ʕ�����
	va_list	args;									// ����

	// �\��������쐬
	va_start(args, pFormat);
	_vsnprintf_s(&aStr[0], sizeof(aStr), _TRUNCATE, pFormat, args);
	va_end(args);

	sprintf_s(&aStrResult[0], sizeof(aStrResult), "!!!!ASSERT ERROR!!!! \nExpressinon %s,\nMessage <%s>\nErrorFile:%s=> Line:%d\n", pExp, &aStr[0], pFileName, nLine);

	//�f�o�b�K�ɏo�͂��邱�ƂŃG���[���̃R�s�[�y�[�X�g���o����B
#ifdef OUTPUT_ASSERT_STRING
	OutputDebugString(&aStrResult[0]);
#endif

	//���b�Z�[�W�{�b�N�X�ɏo���ƃv���O���}�ɂƂ��āA�󋵂�c�����₷��
#ifdef OUTPUT_ASSERT_BOX
	MessageBox(NULL, &aStrResult[0], "ASSERT ERROR", MB_OK);
#endif
}


//EOF