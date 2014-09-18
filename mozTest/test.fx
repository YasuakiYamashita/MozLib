float4 gColor;
float4x4 proj;

float3 pos = { 0, 0, 0};
float rot = 0;

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

	float sinBuf = sin(rot); // サインバッファ
	float cosBuf = cos(rot); // コサインバッファ
	float3 pos2 = Pos;

	pos2.x = Pos.x * cosBuf - Pos.y * sinBuf;
	pos2.y = Pos.x * sinBuf + Pos.y * cosBuf;
	pos2.x += pos.x;
	pos2.y += pos.y;

	Out.Pos = mul(float4(pos2, 1.0f), proj);
	Out.color = Col;
	Out.uv = Tex0;

	return Out;
}

// ピクセルシェーダ
float4 PS(Vs_out vr) : COLOR
{
	return vr.color;
}


// ピクセルシェーダ
float4 PS2(Vs_out vr) : COLOR
{
	return vr.color * tex2D(diffuseSampler, vr.uv);
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
	pass p1
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();

		// レンダーステート系も使える！！
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

	}

}