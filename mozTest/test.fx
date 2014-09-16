float4 gColor;
float4x4 proj;

// テクスチャ
texture gDiffuseMap;

struct Vs_out
{
	float4 Pos	 : POSITION;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
};

// テクスチャサンプラー
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

Vs_out VS(
	float3 Pos    : POSITION,          // モデルの頂点
	float4 Col : COLOR0,             // 法線ベクトル
	float2 Tex0 : TEXCOORD0	         // テクスチャ座標
	)
{
	Vs_out Out;
	Out.Pos = mul(float4(Pos, 1.0f), proj);
	Out.color = gColor;
	Out.uv = Tex0;

	return Out;
}

// ピクセルシェーダ
float4 PS(Vs_out vr) : COLOR
{
	// テクスチャ適用
	return vr.color;
}

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