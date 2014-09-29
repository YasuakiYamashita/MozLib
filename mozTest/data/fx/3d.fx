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

float4x4 mWLP;		// ���[�J������ˉe��Ԃւ̍��W�ϊ�
float4x4 mWLPB;		// �e�N�X�`�����W�n�ւ̎ˉe
float4   vCol = { 1, 1, 1, 1 }; // ���b�V���̐F
float4	 vLightDir;	// ���C�g�̕���

static float2 gScreenSize = { 1280.0f, 720.0f };
static float MAP_SIZE = 1024.0f;
static float COL_SIZE = 65535.f;

//==============================================================================
// �\����
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
// �o�[�e�b�N�X�V�F�[�_
//------------------------------------------------------------------------------
VS_OUT VS(VS_IN vdata)
{
	// �\���̐錾
	VS_OUT result = (VS_OUT)0;

	result.posH = mul(float4(vdata.posL, 1.0f), gMatWVP);

	result.color = vdata.color;
	result.uv = vdata.uv;
	result.pos = result.posH;

	return result;
}

//==============================================================================
// �s�N�Z���V�F�[�_
//------------------------------------------------------------------------------
float4 PS(VS_OUT vr) : COLOR
{
	return vr.color;
}

//==============================================================================
// �s�N�Z���V�F�[�_
//------------------------------------------------------------------------------
float4 PS_TEX(VS_OUT vr) : COLOR
{
	return tex2D(diffuseSampler, vr.uv) * vr.color;
}

//==============================================================================
// �e�N�j�b�N
// �ʏ�`��
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
		PixelShader = compile ps_2_0 PS_TEX();

		// �����_�[�X�e�[�g
		ZENABLE				= TRUE;
		AlphaBlendEnable	= true;
		BlendOp				= ADD;
		SrcBlend			= SRCALPHA;
		DestBlend			= INVSRCALPHA;
	}
}

//==============================================================================
// shadow�}�b�v�p
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
// ���_�V�F�[�_�v���O����
//------------------------------------------------------------------------------
VS_OUT_SHADOW VS_shadowMap(float4 Pos : POSITION){
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW)0;        // �o�̓f�[�^

	// ���W�ϊ�
	float4 pos = mul(Pos, mWLP);

	// �ʒu���W
	Out.Pos = pos;

	// �J�������W�n�ł̐[�x���e�N�X�`���ɓ����
	Out.Depth = pos.zzzw;

	return Out;
}

//==============================================================================
// �s�N�Z���V�F�[�_�v���O����
//------------------------------------------------------------------------------
float4 PS_shadowMap(VS_OUT_SHADOW In) : COLOR
{
	float depth = In.Depth.z / In.Depth.w;
	float4 unpacked_depth = float4(0, 0, COL_SIZE, COL_SIZE);
	unpacked_depth.g  = modf(depth*COL_SIZE, unpacked_depth.r);
	unpacked_depth.b *= modf(unpacked_depth.g*COL_SIZE, unpacked_depth.g);
	return unpacked_depth / COL_SIZE;  // �W����
}


//==============================================================================
// �e�N�j�b�N
// shadow�}�b�v�p
//------------------------------------------------------------------------------
technique ShadowMapTechnique
{
	// �e�N�X�`������
	pass p0
	{
		VertexShader = compile vs_2_0 VS_shadowMap();
		PixelShader = compile ps_2_0 PS_shadowMap();

		// �����_�[�X�e�[�g
		ZENABLE = TRUE;
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}

	// �e�N�X�`���K�p
	pass p1
	{
		VertexShader = compile vs_2_0 VS_shadowMap();
		PixelShader = compile ps_2_0 PS_shadowMap();

		// �����_�[�X�e�[�g
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
// ���_�V�F�[�_�v���O����
//------------------------------------------------------------------------------
VS_OUTPUT_QUAD VS_edgeMap(
	float4 Pos : POSITION,          // ���f���̒��_
	float2 Tex : TEXCOORD0	         // �e�N�X�`�����W
	){
	VS_OUTPUT_QUAD Out = (VS_OUTPUT_QUAD)0;        // �o�̓f�[�^

	// �ʒu���W
	Out.Pos = Pos;

	Out.Tex0 = Tex + float2(-0.5f / MAP_SIZE, -0.5f / MAP_SIZE);
	Out.Tex1 = Tex + float2(+0.5f / MAP_SIZE, +0.5f / MAP_SIZE);
	Out.Tex2 = Tex + float2(-0.5f / MAP_SIZE, +0.5f / MAP_SIZE);
	Out.Tex3 = Tex + float2(+0.5f / MAP_SIZE, -0.5f / MAP_SIZE);

	return Out;
}

//==============================================================================
// �s�N�Z���V�F�[�_�v���O����
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

	return d / COL_SIZE;  // �W����

}

//==============================================================================
// �e�N�j�b�N
// edge�}�b�v�p
//------------------------------------------------------------------------------
technique EdgeMapTechnique
{
	// �e�N�X�`������
	pass p0
	{
		VertexShader = compile vs_2_0 VS_edgeMap();
		PixelShader = compile ps_2_0 PS_edgeMap();

		// �����_�[�X�e�[�g
		ZENABLE = FALSE;
	}
}

//==============================================================================
// ���_�V�F�[�_�v���O����
//------------------------------------------------------------------------------
VS_OUTPUT_QUAD VS_edgeSmudge(
	float4 Pos : POSITION,          // ���f���̒��_
	float2 Tex : TEXCOORD0	         // �e�N�X�`�����W
	){
	VS_OUTPUT_QUAD Out = (VS_OUTPUT_QUAD)0;        // �o�̓f�[�^

	// �ʒu���W
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
// �s�N�Z���V�F�[�_�v���O����
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
// �e�N�j�b�N
// edge�ڂ����p
//------------------------------------------------------------------------------
technique EdgeSmudgeTechnique
{
	// �e�N�X�`������
	pass p0
	{
		VertexShader = compile vs_2_0 VS_edgeSmudge();
		PixelShader = compile ps_2_0 PS_edgeSmudge();

		// �����_�[�X�e�[�g
		ZENABLE = FALSE;
	}
}

//==============================================================================
// ���_�V�F�[�_�v���O����
//------------------------------------------------------------------------------
VS_OUT_SHADOW VS_shadow(VS_IN In){
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW)0;        // �o�̓f�[�^

	// ���W�ϊ�
	Out.Pos = mul(float4(In.posL, 1), gMatWVP);
	// �F
	Out.Diffuse = vCol * max(dot(vLightDir, float4(In.nor, 1)), 0);// �g�U�F
	Out.Ambient = vCol * 0.3f;                     // ���F

	Out.uv = In.uv;
	// �e�N�X�`�����W
	Out.ShadowMapUV = mul(float4(In.posL, 1), mWLPB);
	Out.Depth       = mul(float4(In.posL, 1), mWLP);
	return Out;
}
//==============================================================================
// �s�N�Z���V�F�[�_�v���O����
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
// �e�N�j�b�N
// �e���p�ʏ�`��
//------------------------------------------------------------------------------
technique shadowTechnique
{
	// �e�N�X�`������
	pass p0
	{
		VertexShader = compile vs_2_0 VS_shadow();
		PixelShader = compile ps_2_0 PS_shadow();

		// �����_�[�X�e�[�g
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = ZERO;
		DestBlend = INVSRCCOLOR;
		ZENABLE = true;

	}

	// �e�N�X�`���K�p
	pass p1
	{
		VertexShader = compile vs_2_0 VS_shadow();
		PixelShader = compile ps_2_0 PS_shadow();

		// �����_�[�X�e�[�g
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = ZERO;
		DestBlend = INVSRCCOLOR;
		ZENABLE = true;
	}
}