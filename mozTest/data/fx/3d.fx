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

float4x4 mWLP;		// ローカルから射影空間への座標変換
float4x4 mWLPB;		// テクスチャ座標系への射影
float4   vCol = { 1, 1, 1, 1 }; // メッシュの色
float4	 vLightDir;	// ライトの方向

static float2 gScreenSize = { 1280.0f, 720.0f };
static float MAP_SIZE = 1024.0f;
static float COL_SIZE = 65535.f;

//==============================================================================
// 構造体
//------------------------------------------------------------------------------
struct VS_IN
{
	float3 posL  : POSITION0;
	float3 nor   : NORMAL0;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
};
struct VS_OUT
{
	float4 posH  : POSITION;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
	float4 pos    : TEXCOORD1;
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

texture SrcMap;
sampler SrcSamp = sampler_state
{
	Texture = <SrcMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture ShadowMap;
sampler ShadowMapSamp = sampler_state
{
	Texture = <ShadowMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};

//==============================================================================
// バーテックスシェーダ
//------------------------------------------------------------------------------
VS_OUT VS(VS_IN vdata)
{
	// 構造体宣言
	VS_OUT result = (VS_OUT)0;

	result.posH = mul(float4(vdata.posL, 1.0f), gMatWVP);

	result.color = vdata.color;
	result.uv = vdata.uv;
	result.pos = result.posH;

	return result;
}

//==============================================================================
// ピクセルシェーダ
//------------------------------------------------------------------------------
float4 PS(VS_OUT vr) : COLOR
{
	return vr.color;
}

//==============================================================================
// ピクセルシェーダ
//------------------------------------------------------------------------------
float4 PS_TEX(VS_OUT vr) : COLOR
{
	return tex2D(diffuseSampler, vr.uv) * vr.color;
}

//==============================================================================
// テクニック
// 通常描画
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
		PixelShader = compile ps_2_0 PS_TEX();

		// レンダーステート
		ZENABLE				= TRUE;
		AlphaBlendEnable	= true;
		BlendOp				= ADD;
		SrcBlend			= SRCALPHA;
		DestBlend			= INVSRCALPHA;
	}
}

//==============================================================================
// shadowマップ用
//------------------------------------------------------------------------------
struct VS_OUT_SHADOW
{
	float4 Pos			: POSITION;
	float4 Diffuse		: COLOR0;
	float4 Ambient		: COLOR1;
	float2 uv			: TEXCOORD0;
	float4 Depth		: TEXCOORD1;
	float4 ShadowMapUV	: TEXCOORD2;
};

//==============================================================================
// 頂点シェーダプログラム
//------------------------------------------------------------------------------
VS_OUT_SHADOW VS_shadowMap(float4 Pos : POSITION){
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW)0;        // 出力データ

	// 座標変換
	float4 pos = mul(Pos, mWLP);

	// 位置座標
	Out.Pos = pos;

	// カメラ座標系での深度をテクスチャに入れる
	Out.Depth = pos.zzzw;

	return Out;
}

//==============================================================================
// ピクセルシェーダプログラム
//------------------------------------------------------------------------------
float4 PS_shadowMap(VS_OUT_SHADOW In) : COLOR
{
	float depth = In.Depth.z / In.Depth.w;
	float4 unpacked_depth = float4(0, 0, COL_SIZE, COL_SIZE);
	unpacked_depth.g  = modf(depth*COL_SIZE, unpacked_depth.r);
	unpacked_depth.b *= modf(unpacked_depth.g*COL_SIZE, unpacked_depth.g);
	return unpacked_depth / COL_SIZE;  // 標準化
}


//==============================================================================
// テクニック
// shadowマップ用
//------------------------------------------------------------------------------
technique ShadowMapTechnique
{
	// テクスチャ無し
	pass p0
	{
		VertexShader = compile vs_2_0 VS_shadowMap();
		PixelShader = compile ps_2_0 PS_shadowMap();

		// レンダーステート
		ZENABLE = TRUE;
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}

	// テクスチャ適用
	pass p1
	{
		VertexShader = compile vs_2_0 VS_shadowMap();
		PixelShader = compile ps_2_0 PS_shadowMap();

		// レンダーステート
		ZENABLE = TRUE;
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}
}


//==============================================================================
// 
//------------------------------------------------------------------------------
struct VS_OUTPUT_QUAD
{
	float4 Pos			: POSITION;
	float2 Tex0			: TEXCOORD0;
	float2 Tex1			: TEXCOORD1;
	float2 Tex2			: TEXCOORD2;
	float2 Tex3			: TEXCOORD3;
	float2 Tex4			: TEXCOORD4;
	float2 Tex5			: TEXCOORD5;
	float2 Tex6			: TEXCOORD6;
	float2 Tex7			: TEXCOORD7;
};

//==============================================================================
// 頂点シェーダプログラム
//------------------------------------------------------------------------------
VS_OUTPUT_QUAD VS_edgeMap(
	float4 Pos : POSITION,          // モデルの頂点
	float2 Tex : TEXCOORD0	         // テクスチャ座標
	){
	VS_OUTPUT_QUAD Out = (VS_OUTPUT_QUAD)0;        // 出力データ

	// 位置座標
	Out.Pos = Pos;

	Out.Tex0 = Tex + float2(-0.5f / MAP_SIZE, -0.5f / MAP_SIZE);
	Out.Tex1 = Tex + float2(+0.5f / MAP_SIZE, +0.5f / MAP_SIZE);
	Out.Tex2 = Tex + float2(-0.5f / MAP_SIZE, +0.5f / MAP_SIZE);
	Out.Tex3 = Tex + float2(+0.5f / MAP_SIZE, -0.5f / MAP_SIZE);

	return Out;
}

//==============================================================================
// ピクセルシェーダプログラム
//------------------------------------------------------------------------------
float4 PS_edgeMap(VS_OUTPUT_QUAD In) : COLOR
{
	float3 Color = 0;
	float4 d = 0;

	Color = tex2D(SrcSamp, In.Tex0).rgb;
	d.r = Color.r + ((Color.g + (Color.b / COL_SIZE)) / COL_SIZE);
	Color = tex2D(SrcSamp, In.Tex1).rgb;
	d.g = Color.r + ((Color.g + (Color.b / COL_SIZE)) / COL_SIZE);
	Color = tex2D(SrcSamp, In.Tex2).rgb;
	d.b = Color.r + ((Color.g + (Color.b / COL_SIZE)) / COL_SIZE);
	Color = tex2D(SrcSamp, In.Tex3).rgb;
	d.a = Color.r + ((Color.g + (Color.b / COL_SIZE)) / COL_SIZE);

	Color.r = d.r - d.g;
	Color.g = d.b - d.a;

	d    = float4(0, 0, COL_SIZE, COL_SIZE);
	d.g  = modf((Color.r*Color.r + Color.g*Color.g)*COL_SIZE, d.r);
	d.b *= modf(d.g*COL_SIZE, d.g);

	return d / COL_SIZE;  // 標準化

}

//==============================================================================
// テクニック
// edgeマップ用
//------------------------------------------------------------------------------
technique EdgeMapTechnique
{
	// テクスチャ無し
	pass p0
	{
		VertexShader = compile vs_2_0 VS_edgeMap();
		PixelShader = compile ps_2_0 PS_edgeMap();

		// レンダーステート
		ZENABLE = FALSE;
	}
}

//==============================================================================
// 頂点シェーダプログラム
//------------------------------------------------------------------------------
VS_OUTPUT_QUAD VS_edgeSmudge(
	float4 Pos : POSITION,          // モデルの頂点
	float2 Tex : TEXCOORD0	         // テクスチャ座標
	){
	VS_OUTPUT_QUAD Out = (VS_OUTPUT_QUAD)0;        // 出力データ

	// 位置座標
	Out.Pos = Pos;

	Out.Tex0 = Tex + float2(-3.0f / MAP_SIZE, -3.0f / MAP_SIZE);
	Out.Tex1 = Tex + float2(-3.0f / MAP_SIZE, -1.0f / MAP_SIZE);
	Out.Tex2 = Tex + float2(-3.0f / MAP_SIZE, 1.0f / MAP_SIZE);
	Out.Tex3 = Tex + float2(-3.0f / MAP_SIZE, 3.0f / MAP_SIZE);
	Out.Tex4 = Tex + float2(-1.0f / MAP_SIZE, -3.0f / MAP_SIZE);
	Out.Tex5 = Tex + float2(-1.0f / MAP_SIZE, -1.0f / MAP_SIZE);
	Out.Tex6 = Tex + float2(-1.0f / MAP_SIZE, 1.0f / MAP_SIZE);
	Out.Tex7 = Tex + float2(-1.0f / MAP_SIZE, 3.0f / MAP_SIZE);

	return Out;
}

//==============================================================================
// ピクセルシェーダプログラム
//------------------------------------------------------------------------------
float4 PS_edgeSmudge(VS_OUTPUT_QUAD In) : COLOR
{
	float4 Color;
	float2 dvu = float2(4.0f / MAP_SIZE, 0);

	Color = tex2D(SrcSamp, In.Tex0)
		+ tex2D(SrcSamp, In.Tex1)
		+ tex2D(SrcSamp, In.Tex2)
		+ tex2D(SrcSamp, In.Tex3)
		+ tex2D(SrcSamp, In.Tex4)
		+ tex2D(SrcSamp, In.Tex5)
		+ tex2D(SrcSamp, In.Tex6)
		+ tex2D(SrcSamp, In.Tex7)
		+ tex2D(SrcSamp, In.Tex0 + dvu)
		+ tex2D(SrcSamp, In.Tex1 + dvu)
		+ tex2D(SrcSamp, In.Tex2 + dvu)
		+ tex2D(SrcSamp, In.Tex3 + dvu)
		+ tex2D(SrcSamp, In.Tex4 + dvu)
		+ tex2D(SrcSamp, In.Tex5 + dvu)
		+ tex2D(SrcSamp, In.Tex6 + dvu)
		+ tex2D(SrcSamp, In.Tex7 + dvu)
		;

	return 0.7f * Color;
}

//==============================================================================
// テクニック
// edgeぼかす用
//------------------------------------------------------------------------------
technique EdgeSmudgeTechnique
{
	// テクスチャ無し
	pass p0
	{
		VertexShader = compile vs_2_0 VS_edgeSmudge();
		PixelShader = compile ps_2_0 PS_edgeSmudge();

		// レンダーステート
		ZENABLE = FALSE;
	}
}

//==============================================================================
// 頂点シェーダプログラム
//------------------------------------------------------------------------------
VS_OUT_SHADOW VS_shadow(VS_IN In){
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW)0;        // 出力データ

	// 座標変換
	Out.Pos = mul(float4(In.posL, 1), gMatWVP);
	// 色
	Out.Diffuse = vCol * max(dot(vLightDir, float4(In.nor, 1)), 0);// 拡散色
	Out.Ambient = vCol * 0.3f;                     // 環境色

	Out.uv = In.uv;
	// テクスチャ座標
	Out.ShadowMapUV = mul(float4(In.posL, 1), mWLPB);
	Out.Depth       = mul(float4(In.posL, 1), mWLP);
	return Out;
}
//==============================================================================
// ピクセルシェーダプログラム
//------------------------------------------------------------------------------
float4 PS_shadow(VS_OUT_SHADOW In) : COLOR
{
	float4 Color = In.Ambient;
	float3 c = tex2Dproj(ShadowMapSamp, In.ShadowMapUV).rgb;
	float shadow_map = c.r + ((c.g + (c.b / COL_SIZE)) / COL_SIZE);
	c = tex2Dproj(SrcSamp, In.ShadowMapUV);
	float shadow = c.r + ((c.g + (c.b / COL_SIZE)) / COL_SIZE);

	Color += ((shadow_map < (In.Depth.z / In.Depth.w) - 0.005f) ? shadow + ((In.Depth.z / In.Depth.w) - shadow_map + 0.005f) : 1);
	Color.a = 0;

	Color = (Color - 1) * -1;

	return Color;
}

//==============================================================================
// テクニック
// 影つけ用通常描画
//------------------------------------------------------------------------------
technique shadowTechnique
{
	// テクスチャ無し
	pass p0
	{
		VertexShader = compile vs_2_0 VS_shadow();
		PixelShader = compile ps_2_0 PS_shadow();

		// レンダーステート
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = ZERO;
		DestBlend = INVSRCCOLOR;
		ZENABLE = true;

	}

	// テクスチャ適用
	pass p1
	{
		VertexShader = compile vs_2_0 VS_shadow();
		PixelShader = compile ps_2_0 PS_shadow();

		// レンダーステート
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = ZERO;
		DestBlend = INVSRCCOLOR;
		ZENABLE = true;
	}
}