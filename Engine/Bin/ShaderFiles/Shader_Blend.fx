
float		g_fFarPlane;

matrix		g_matLightView;
matrix		g_matProj;

matrix		g_matViewInv;
matrix		g_matProjInv;



texture		g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_ShadeTexture;

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_ShadowTexture;
sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;
	minfilter = linear;
	magfilter = linear;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};

texture		g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;

	//BorderColor = 0xFFFFFFFF;
};


struct PS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector	vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector	vShade = tex2D(ShadeSampler, In.vTexUV);
	vector	vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vSpecular.a = 0.f;

	Out.vColor = vDiffuse * vShade + vSpecular;


	vector	vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	//현재 픽셀의 좌표를 월드로 변환
	float  vViewZ = vDepthInfo.y * g_fFarPlane;
	vector vProjPos;
	vProjPos.x = (In.vTexUV.x * 2.f - 1.f) * vViewZ;
	vProjPos.y = (In.vTexUV.y * -2.f + 1.f) * vViewZ;
	vProjPos.z = vDepthInfo.x * vViewZ;
	vProjPos.w = vViewZ;

	vector	vViewPos;
	vViewPos = mul(vProjPos, g_matProjInv);

	vector	vWorldPos;
	vWorldPos = mul(vViewPos, g_matViewInv);

	vector	vLightViewPos;
	vLightViewPos = mul(vWorldPos, g_matLightView);
	vector	vLightProjPos;
	vLightProjPos = mul(vLightViewPos, g_matProj);

	//0~1
	float fDepth = vLightProjPos.z / vLightProjPos.w;

	//0~1
	float2	UV = vLightProjPos.xy / vLightProjPos.w;
	UV.y = -UV.y;
	UV = UV * 0.5f + 0.5f;
//	UV = saturate(UV);
	
	vector	vShadowDepth = tex2D(ShadowSampler, UV);

	if (fDepth > vShadowDepth.x + 0.0000125f)
		Out.vColor *= 0.5f;

//	In.Ambient + ((shadow * In.Depth.w < In.Depth.z - 0.03f) ? 0 : In.Diffuse);

	return Out;
}

technique Default_Device
{
	pass Default
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}