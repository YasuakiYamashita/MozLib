//==============================================================================
//
// �A�T�[�g�o���p[mozAssert.h]
// Author : Yasuaki Yamashita : 2014/07/29
//
//==============================================================================
#pragma once

#ifndef _MOZASSERT_H_
#define _MOZASSERT_H_

//==============================================================================
// �C���N���[�h
//------------------------------------------------------------------------------
#include <stdarg.h>

//==============================================================================
// �}�N����`
//------------------------------------------------------------------------------
#define BLAKEPOINT __asm int 3	//�����I�u���[�N��������A�Z���u������
#define CUSTOM_ASSERT_ENABLE	//�J�X�^���p�̃A�T�[�g�����邱�Ƃ��O���ɒm�点�Ă�����

//�J�X�^�}�C�Y�}�N��
//�K�v�ɉ����ā@�R�����g�A�E�g�������Đ؂�ւ��Ă��������B
#define OUTPUT_ASSERT_STRING //��`����Ă�ƃA�T�[�g�����f�o�b�K�̏o�͂ɂ͂��Ă���� 
#define OUTPUT_ASSERT_BOX	 //��`����Ă�ƃA�T�[�g����MessageBox�Œm�点��

//DO_ASSERT �Ɓ@DO_STORONG_ASSERT��
//�R�����g�A�E�g����Ƃ��ł��A�T�[�g�̗L���؂�ւ����ł���
#ifdef _DEBUG
#ifndef DO_ASSERT
#define DO_ASSERT
#endif
#ifndef DO_STORONG_ASSERT
#define DO_STORONG_ASSERT
#endif
#endif

#ifdef DO_ASSERT
#define ASSERT(exp,format,...) if (exp){}else{\
	MyAssert(#exp, __FILE__, __LINE__, format, __VA_ARGS__); \
	BLAKEPOINT/*�u���[�N�|�C���g�Ăяo��*/\
}
#else
#define ASSERT(exp,format,...) (exp)
#endif

#ifdef DO_STORONG_ASSERT
#define STRONG_ASSERT(exp,format,...) if (exp){}else{\
	MyAssert(#exp, __FILE__, __LINE__, format, __VA_ARGS__); \
	BLAKEPOINT/*�u���[�N�|�C���g�Ăяo��*/\
}
#else
#define STRONG_ASSERT(exp,format,...) (exp)
#endif

void MyAssert(char *pExp, char *pFileName, int nLine, char *pFormat, ...);


#endif
//EOF