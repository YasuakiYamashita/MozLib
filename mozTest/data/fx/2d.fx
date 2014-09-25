//==============================================================================
//
// 2D用シェーダファイル[.cpp]
// Author : Yasuaki Yamashita : 2014/09/18
//
//==============================================================================

//==============================================================================
// シェーダファイル
//------------------------------------------------------------------------------
float4x4 mtxProj;
float4x4 mtxWorld;
float2 gScreenSize = { 1280.0f, 720.0f };

//------------------------------------------------------------------------------
// テクスチャ
//------------------------------------------------------------------------------
texture gDiffuseMap;
sampler diffuseSampler = sampler_state
{
	Texture = <gDiffuseMap>;

	// トライリニアフィルタリング
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

//==============================================================================
//  構造体
//------------------------------------------------------------------------------
// VS in
struct VS_in
{
	float3 Pos  : POSITION;		// モデルの頂点
	float4 Col  : COLOR0;		// 法線ベクトル
	float2 Tex0 : TEXCOORD0;	// テクスチャ座標
};

// VS -> PS
struct Vs_out
{
	float4 Pos   : POSITION;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
};

//==============================================================================
// 頂点シェーダ
//------------------------------------------------------------------------------
Vs_out VS(VS_in inp)
{
	Vs_out Out;

	Out.Pos   = mul(float4(inp.Pos, 1.0f), mtxWorld);
	Out.Pos   = mul(Out.Pos, mtxProj);

	// スクリーン座標に対して(-0.5, -0.5)のオフセット
	Out.Pos /= Out.Pos.w;

	// 射影空間の座標を一度スクリーン座標にして、
	// 0.5分オフセットしてからもとに戻します。
	// 下のように書いてもきっとコンパイラがうまいこと最適化してくれます（笑）
	Out.Pos.x = (Out.Pos.x * gScreenSize.x - 0.5f) / gScreenSize.x;
	Out.Pos.y = (Out.Pos.y * gScreenSize.y + 0.5f) / gScreenSize.y;

	Out.Pos.w = 1.f;


	Out.color = inp.Col;
	Out.uv    = inp.Tex0;

	return Out;
}

//==============================================================================
// ピクセルシェーダ
//------------------------------------------------------------------------------
float4 PS(Vs_out vr) : COLOR
{
	return vr.color;
}

//==============================================================================
// ピクセルシェーダ　テクスチャ適応
//------------------------------------------------------------------------------
float4 PS2(Vs_out vr) : COLOR
{
	return vr.color * tex2D(diffuseSampler, vr.uv);
}

//==============================================================================
// テクニック
//------------------------------------------------------------------------------
technique basicTechnique
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();

		// レンダーステート系も使える！！
		ZENABLE = FALSE;
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

	}
	pass p1
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();

		// レンダーステート系も使える！！
		ZENABLE = FALSE;
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

	}

}