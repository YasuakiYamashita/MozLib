//==============================================================================
//
// 3Dポリゴン作成[moz3DPolygon.h]
// Author : Yasuaki Yamashita : 2014/09/24
//
//==============================================================================

#pragma once

#ifndef _MOZ3DPOLYGON_H_
#define _MOZ3DPOLYGON_H_

//==============================================================================
// include
//------------------------------------------------------------------------------
#include <mozPolygon.h>

namespace moz
{ 
	namespace DirectX
	{
		//==============================================================================
		// ビルボード用
		//------------------------------------------------------------------------------
		class PolygonBillboard : public Polygon3D
		{
		public:
			PolygonBillboard(const Vector2D& size);
			virtual ~PolygonBillboard();
			
			virtual void SetVtx(const _3DLOCKBUFF* buff);
			virtual void Draw(void);
			virtual void Update(void);
		};
	}
}
#endif
//EOF