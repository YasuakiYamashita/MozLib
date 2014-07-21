//==============================================================================
//
// �Ƃ肠�����R���̓��[�h���Ă����t�@�C��[mozCommon.h]
// Author : Yasuaki Yamashita : 2014/07/10
//
//==============================================================================

#pragma once

#ifndef _MOZCOMMON_H_
#define _MOZCOMMON_H_

//==============================================================================
// STL�̃f�o�b�O�@�\��OFF�ɂ���
//------------------------------------------------------------------------------
#ifndef _SECURE_SCL
#define _SECURE_SCL (0)
#endif
#ifndef _HAS_ITERATOR_DEBUGGING
#define _HAS_ITERATOR_DEBUGGING (0)
#endif
#ifndef  _ITERATOR_DEBUG_LEVEL
#define  _ITERATOR_DEBUG_LEVEL 0
#endif


//==============================================================================
// �}�N���֐���`
//------------------------------------------------------------------------------
// �Z�[�t�e�B���
// ������NULL�łȂ���΁ARelease�֐����ĂсANULL��������
#define SAFE_RELEASE(xInstance)	if((xInstance)!=NULL){(xInstance)->Release();(xInstance)=NULL;}

// �Z�[�t�e�B�폜
// ������NULL�łȂ���΁Adelete���s���ANULL��������
#define SAFE_DELETE(xInstance)	if((xInstance)!=NULL){delete (xInstance);(xInstance)=NULL;}

// �Z�[�t�e�B�z��폜
// ������NULL�łȂ���΁Adelete[]���s���ANULL��������
#define SAFE_DELETE_ARRAY(xInstance)	if((xInstance)!=NULL){delete[] (xInstance);(xInstance)=NULL;}

// ����v�Z�}�N��
#ifndef POW2
#define POW2(x) ((x)*(x))
#endif

// ��Βl�v�Z�}�N��
#ifndef ABS
#define ABS(n)		((n)<0?-(n):(n))
#endif

#endif
//EOF