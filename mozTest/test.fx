float4 gColor;
float4x4 proj;

// �e�N�X�`��
texture gDiffuseMap;

struct Vs_out
{
	float4 Pos	 : POSITION;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
};

// �e�N�X�`���T���v���[
sampler diffuseSampler = sampler_state
{	
	Texture = <gDiffuseMap>;

	// �g���C���j�A�t�B���^�����O
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

Vs_out VS(
	float3 Pos    : POSITION,          // ���f���̒��_
	float4 Col : COLOR0,             // �@���x�N�g��
	float2 Tex0 : TEXCOORD0	         // �e�N�X�`�����W
	)
{
	Vs_out Out;
	Out.Pos = mul(float4(Pos, 1.0f), proj);
	Out.color = gColor;
	Out.uv = Tex0;

	return Out;
}

// �s�N�Z���V�F�[�_
float4 PS(Vs_out vr) : COLOR
{
	// �e�N�X�`���K�p
	return vr.color;
}

technique basicTechnique
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();

		// �����_�[�X�e�[�g�n���g����I�I
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

	}

}