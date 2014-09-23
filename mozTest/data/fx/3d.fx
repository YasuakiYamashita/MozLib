//==============================================================================
//
// 3D用シェーダファイル[3d.fx]
// Author : Yasuaki Yamashita : 2014/09/19
//
//==============================================================================


//------------------------------------------------------------------------------
// グローバル
//------------------------------------------------------------------------------
float4x4 gMatWVP;

//==============================================================================
// 構造体
//------------------------------------------------------------------------------
struct Vs_in
{
	float3 posL  : POSITION0;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
};
struct Vs_out
{
	float4 posH  : POSITION;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
};

//==============================================================================
// テクスチャサンプラー
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
// バーテックスシェーダ
//------------------------------------------------------------------------------
Vs_out VS(Vs_in vdata)
{
	// 構造体宣言
	Vs_out result;
	result.posH = mul(float4(vdata.posL, 1.0f), gMatWVP);
	result.color = vdata.color;
	result.uv = vdata.uv;

	return result;
}

//==============================================================================
// ピクセルシェーダ
//------------------------------------------------------------------------------
float4 PS(Vs_out vr) : COLOR
{
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return vr.color;

	// テクスチャ適応
	//return float4(tex2D(diffuseSampler, vr.uv).rgb, 1.0f);
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
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;

		DestBlend = INVSRCALPHA;
	}
}