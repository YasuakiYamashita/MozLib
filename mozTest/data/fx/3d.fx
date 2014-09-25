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
float2 gScreenSize = { 1280.0f, 720.0f };


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
	return vr.color;
}

//==============================================================================
// ピクセルシェーダ
//------------------------------------------------------------------------------
float4 PS2(Vs_out vr) : COLOR
{
	// テクスチャ適応
	return tex2D(diffuseSampler, vr.uv) * vr.color;
}

//==============================================================================
// テクニック
//------------------------------------------------------------------------------
technique basicTechnique
{
	// テクスチャ無し
	pass p0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();

		// レンダーステート
		ZENABLE				= TRUE;
		AlphaBlendEnable	= true;
		BlendOp				= ADD;
		SrcBlend			= SRCALPHA;
		DestBlend			= INVSRCALPHA;
	}

	// テクスチャ適用
	pass p1
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();

		// レンダーステート
		ZENABLE				= TRUE;
		AlphaBlendEnable	= true;
		BlendOp				= ADD;
		SrcBlend			= SRCALPHA;
		DestBlend			= INVSRCALPHA;
	}
}