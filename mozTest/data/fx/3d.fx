//==============================================================================
//
// 3D�p�V�F�[�_�t�@�C��[3d.fx]
// Author : Yasuaki Yamashita : 2014/09/19
//
//==============================================================================

//------------------------------------------------------------------------------
// �O���[�o��
//------------------------------------------------------------------------------
float4x4 gMatWVP;
float2 gScreenSize = { 1280.0f, 720.0f };


//==============================================================================
// �\����
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
// �e�N�X�`���T���v���[
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
// �o�[�e�b�N�X�V�F�[�_
//------------------------------------------------------------------------------
Vs_out VS(Vs_in vdata)
{
	// �\���̐錾
	Vs_out result;
	
	result.posH = mul(float4(vdata.posL, 1.0f), gMatWVP);

	result.color = vdata.color;
	result.uv = vdata.uv;

	return result;
}

//==============================================================================
// �s�N�Z���V�F�[�_
//------------------------------------------------------------------------------
float4 PS(Vs_out vr) : COLOR
{
	return vr.color;
}

//==============================================================================
// �s�N�Z���V�F�[�_
//------------------------------------------------------------------------------
float4 PS2(Vs_out vr) : COLOR
{
	// �e�N�X�`���K��
	return tex2D(diffuseSampler, vr.uv) * vr.color;
}

//==============================================================================
// �e�N�j�b�N
//------------------------------------------------------------------------------
technique basicTechnique
{
	// �e�N�X�`������
	pass p0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();

		// �����_�[�X�e�[�g
		ZENABLE				= TRUE;
		AlphaBlendEnable	= true;
		BlendOp				= ADD;
		SrcBlend			= SRCALPHA;
		DestBlend			= INVSRCALPHA;
	}

	// �e�N�X�`���K�p
	pass p1
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();

		// �����_�[�X�e�[�g
		ZENABLE				= TRUE;
		AlphaBlendEnable	= true;
		BlendOp				= ADD;
		SrcBlend			= SRCALPHA;
		DestBlend			= INVSRCALPHA;
	}
}