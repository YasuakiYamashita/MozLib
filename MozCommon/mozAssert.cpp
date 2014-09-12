//==============================================================================
//
// アサート用[mozAssert.cpp]
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
// アサート
//------------------------------------------------------------------------------
void MyAssert(char *pExp, char *pFileName, int nLine, char *pFormat, ...)
{
	const int STRSIZE = 128;
	char	aStr[STRSIZE];		// 表示文字列
	char aStrResult[512];			// 最終結果文字列
	va_list	args;									// 引数

	// 表示文字列作成
	va_start(args, pFormat);
	_vsnprintf_s(&aStr[0], sizeof(aStr), _TRUNCATE, pFormat, args);
	va_end(args);

	sprintf_s(&aStrResult[0], sizeof(aStrResult), "!!!!ASSERT ERROR!!!! \nExpressinon %s,\nMessage <%s>\nErrorFile:%s=> Line:%d\n", pExp, &aStr[0], pFileName, nLine);

	//デバッガに出力することでエラー文のコピーペーストが出来る。
#ifdef OUTPUT_ASSERT_STRING
	OutputDebugString(&aStrResult[0]);
#endif

	//メッセージボックスに出すとプログラマにとって、状況を把握しやすい
#ifdef OUTPUT_ASSERT_BOX
	MessageBox(NULL, &aStrResult[0], "ASSERT ERROR", MB_OK);
#endif
}


//EOF