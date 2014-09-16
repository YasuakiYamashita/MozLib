//==============================================================================
//
// アサート出す用[mozAssert.h]
// Author : Yasuaki Yamashita : 2014/07/29
//
//==============================================================================
#pragma once

#ifndef _MOZASSERT_H_
#define _MOZASSERT_H_

//==============================================================================
// インクルード
//------------------------------------------------------------------------------
#include <stdarg.h>

//==============================================================================
// マクロ定義
//------------------------------------------------------------------------------
#define BLAKEPOINT __asm int 3	//強制的ブレーク発生特殊アセンブラ命令
#define CUSTOM_ASSERT_ENABLE	//カスタム用のアサートがあることを外部に知らせてあげる

//カスタマイズマクロ
//必要に応じて　コメントアウト等をして切り替えてください。
#define OUTPUT_ASSERT_STRING //定義されてるとアサート文をデバッガの出力にはいてくれる 
#define OUTPUT_ASSERT_BOX	 //定義されてるとアサート文をMessageBoxで知らせる

//DO_ASSERT と　DO_STORONG_ASSERTを
//コメントアウトするといつでもアサートの有無切り替えができる
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
	BLAKEPOINT/*ブレークポイント呼び出し*/\
}
#else
#define ASSERT(exp,format,...) (exp)
#endif

#ifdef DO_STORONG_ASSERT
#define STRONG_ASSERT(exp,format,...) if (exp){}else{\
	MyAssert(#exp, __FILE__, __LINE__, format, __VA_ARGS__); \
	BLAKEPOINT/*ブレークポイント呼び出し*/\
}
#else
#define STRONG_ASSERT(exp,format,...) (exp)
#endif

void MyAssert(char *pExp, char *pFileName, int nLine, char *pFormat, ...);


#endif
//EOF