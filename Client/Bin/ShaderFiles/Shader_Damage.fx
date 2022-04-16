matrix		g_matWorld, g_matView, g_matProj;
float		g_fTimeAcc;

vector		g_vMaterial;

bool		g_bThousands;

texture		g_UnitsTexture;
sampler UnitsSampler = sampler_state
{
	texture = g_UnitsTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_TensTexture;
sampler TensSampler = sampler_state
{
	texture = g_TensTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_HundredsTexture;
sampler HundredsSampler = sampler_state
{
	texture = g_HundredsTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_ThousandsTexture;
sampler ThousandsSampler = sampler_state
{
	texture = g_ThousandsTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4	vPosition :POSITION; // 모든 위치 변환은 종료, 추갖거으로 위치변환 불가
	float2	vTexUV : TEXCOORD;
};

struct PS_OUT
{
	vector	vColor : COLOR;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;


	float2	UV;
	UV.y = In.vTexUV.y;

	if (In.vTexUV.x >= 0.75f)
	{
		
		UV.x = (In.vTexUV.x - 0.75f)*4.f;

		Out.vColor = tex2D(UnitsSampler, UV) * g_vMaterial;

	}
	else if (In.vTexUV.x >= 0.5f)
	{	
		UV.x = (In.vTexUV.x - 0.5f)*4.f;
		Out.vColor = tex2D(TensSampler, UV) * g_vMaterial;
		
	}
	else if (In.vTexUV.x >= 0.25f)
	{
		UV.x = (In.vTexUV.x - 0.25f)*4.f;
		Out.vColor = tex2D(HundredsSampler, UV) * g_vMaterial;

	}
	else
	{
		UV.x = In.vTexUV.x * 4.f;
		Out.vColor = tex2D(ThousandsSampler, UV) * g_vMaterial;

		if (!g_bThousands)
			Out.vColor.a = 0.f;
	}

	Out.vColor.a = Out.vColor.a * (1.f - g_fTimeAcc);

	return Out;
}

technique Default_Device
{
	pass Default_Rendering
	{
		ZWriteEnable = false;
		
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}