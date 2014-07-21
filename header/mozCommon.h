//==============================================================================
//
// とりあえずコレはロードしておけファイル[mozCommon.h]
// Author : Yasuaki Yamashita : 2014/07/10
//
//==============================================================================

#pragma once

#ifndef _MOZCOMMON_H_
#define _MOZCOMMON_H_

//==============================================================================
// STLのデバッグ機能をOFFにする
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
// マクロ関数定義
//------------------------------------------------------------------------------
// セーフティ解放
// 引数がNULLでなければ、Release関数を呼び、NULLを代入する
#define SAFE_RELEASE(xInstance)	if((xInstance)!=NULL){(xInstance)->Release();(xInstance)=NULL;}

// セーフティ削除
// 引数がNULLでなければ、deleteを行い、NULLを代入する
#define SAFE_DELETE(xInstance)	if((xInstance)!=NULL){delete (xInstance);(xInstance)=NULL;}

// セーフティ配列削除
// 引数がNULLでなければ、delete[]を行い、NULLを代入する
#define SAFE_DELETE_ARRAY(xInstance)	if((xInstance)!=NULL){delete[] (xInstance);(xInstance)=NULL;}

// 自乗計算マクロ
#ifndef POW2
#define POW2(x) ((x)*(x))
#endif

// 絶対値計算マクロ
#ifndef ABS
#define ABS(n)		((n)<0?-(n):(n))
#endif

#endif
//EOF