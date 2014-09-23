//==============================================================================
//
// 2D�p�V�F�[�_�t�@�C��[.cpp]
// Author : Yasuaki Yamashita : 2014/09/18
//
//==============================================================================

//==============================================================================
// �V�F�[�_�t�@�C��
//------------------------------------------------------------------------------
float4x4 mtxProj;
float4x4 mtxWorld;

//------------------------------------------------------------------------------
// �e�N�X�`��
//------------------------------------------------------------------------------
texture gDiffuseMap;
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

//==============================================================================
//  �\����
//------------------------------------------------------------------------------
// VS in
struct VS_in
{
	float3 Pos  : POSITION;		// ���f���̒��_
	float4 Col  : COLOR0;		// �@���x�N�g��
	float2 Tex0 : TEXCOORD0;	// �e�N�X�`�����W
};

// VS -> PS
struct Vs_out
{
	float4 Pos   : POSITION;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
};

//==============================================================================
// ���_�V�F�[�_
//------------------------------------------------------------------------------
Vs_out VS(VS_in inp)
{
	Vs_out Out;

	Out.Pos   = mul(float4(inp.Pos, 1.0f), mtxWorld);
	Out.Pos   = mul(Out.Pos, mtxProj);
	Out.color = inp.Col;
	Out.uv    = inp.Tex0;

	return Out;
}

//==============================================================================
// �s�N�Z���V�F�[�_
//------------------------------------------------------------------------------
float4 PS(Vs_out vr) : COLOR
{
	return vr.color;
}

//==============================================================================
// �s�N�Z���V�F�[�_�@�e�N�X�`���K��
//------------------------------------------------------------------------------
float4 PS2(Vs_out vr) : COLOR
{
	return vr.color * tex2D(diffuseSampler, vr.uv);
}

//==============================================================================
// �e�N�j�b�N
//------------------------------------------------------------------------------
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
	pass p1
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();

		// �����_�[�X�e�[�g�n���g����I�I
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

	}

}