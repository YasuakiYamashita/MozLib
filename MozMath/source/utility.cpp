//==============================================================================
//
// 数学系ユーティリティ[utility.cpp]
// Author : Yasuaki Yamashita : 2014/06/30
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "mozMath.h"


namespace moz
{
	namespace math
	{
		//------------------------------------------------------------------------------
		// アングルを正規化
		//------------------------------------------------------------------------------
		float NormalizeAngle(float Angle)
		{
			long ofs = (*(long*)&Angle & 0x80000000) | 0x3F000000;
			return (Angle - ((int)(Angle * kRCPTWOPI + *(float*)&ofs) * kTWOPI));
		}
	}
}



//EOF