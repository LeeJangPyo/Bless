matrix		g_matWorld, g_matView, g_matProj;
float		g_fFarPlane;
float		g_fDetail;


texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
texture		g_SubTexture;

sampler SubSampler = sampler_state
{
	texture = g_SubTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_FilterTexture;

sampler		FilterSampler = sampler_state
{
	texture = g_FilterTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_BaseNormalTexture;
sampler		BaseNormalSampler = sampler_state
{
	texture = g_BaseNormalTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_SubNormalTexture;
sampler		SubNormalSampler = sampler_state
{
	texture = g_SubNormalTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD;
	float4		vTangent : TEXCOORD1;
	
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	vector		vBinormal : TEXCOORD2;
	vector		vTangent : TEXCOORD3;
	vector		vNormal : TEXCOORD4;
};


VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vProjPos = Out.vPosition;

	float3 vBinormal = cross(In.vNormal.xyz, In.vTangent.xyz);

	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vBinormal = normalize(mul(vector(vBinormal, 0.f), g_matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent.xyz, 0.f), g_matWorld));

	return Out;
}


struct PS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	vector		vBinormal : TEXCOORD2;
	vector		vTangent : TEXCOORD3;
	vector		vNormal : TEXCOORD4;
};

struct PS_OUT
{
	vector	vDiffuse : COLOR0;
	vector  vNormal : COLOR1;
	vector	vDepth : COLOR2;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	float2 f2TexUV = In.vTexUV / g_fDetail;

	vector vBaseColor = tex2D(BaseSampler, In.vTexUV /** g_fDetail*/);
	vector vSubColor = tex2D(SubSampler, In.vTexUV /** g_fDetail*/);
	vector vFilter = tex2D(FilterSampler, f2TexUV);

	Out.vDiffuse = vSubColor * vFilter + vBaseColor * (1.f - vFilter);

	vector vTangetNormal;
	if (vFilter.x < 0.5f)
	{
		vTangetNormal = tex2D(BaseNormalSampler, In.vTexUV);
		vTangetNormal = normalize(vTangetNormal * 2 - 1);
	}
	else
	{
		vTangetNormal = tex2D(SubNormalSampler, In.vTexUV);
		vTangetNormal = normalize(vTangetNormal * 2 - 1);
	}
	
	float4x4 matTangent = float4x4(In.vTangent, In.vBinormal, In.vNormal, float4(0.f, 0.f, 0.f, 1.f));

	vector vWorldNormal = mul(vTangetNormal, matTangent);
	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFarPlane, 0.f, 0.f);

	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{
		CullMode = ccw;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}