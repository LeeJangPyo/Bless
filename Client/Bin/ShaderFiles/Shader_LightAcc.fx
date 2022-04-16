// 빛정보.
vector		g_vLightPos;
float		g_fRange;
float		g_fFarPlane;


vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);


//Camera Position
vector		g_vCamPosition;

matrix		g_matViewInv;
matrix		g_matProjInv;

texture		g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

texture		g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture		g_SpecularTexture;
sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
};


struct PS_IN
{
	vector	vPosition :POSITION; 
	float2	vTexUV : TEXCOORD;
};

struct PS_OUT
{
	vector	vShade : COLOR0;
	vector	vSpecular : COLOR1;
};


PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector vNormalInfo = tex2D(NormalSampler,In.vTexUV);
	vector vDepthInfo = tex2D(DepthSampler, In.vTexUV);
	float  vViewZ = vDepthInfo.y * g_fFarPlane;
	vector vWorldNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	Out.vShade = max(dot(normalize(g_vLightDir) * -1.f, vWorldNormal), 0.f);
	Out.vShade.a = 1.f;

	vector vProjPos;
	//UV좌표를 기준으로 -1~1 , 1~-1까지 변환
	//x : 0 -> -1, 1 -> 1		->		Projection 행렬 곱한 위치로 변환	( 범위 0~Far)
	vProjPos.x = (In.vTexUV.x * 2.f - 1.f) * vViewZ;

	//y : 0 -> 1, 1 -> -1 
	vProjPos.y = (In.vTexUV.y * -2.f + 1.f) * vViewZ;

	//0~1의 범위 z값에 w값을 곱해 projetion 행렬을 곱한 위치까지로 변환
	vProjPos.z = vDepthInfo.x * vViewZ;

	//z값 복원
	vProjPos.w = vViewZ;

	vector			vViewPos;
	vViewPos = mul(vProjPos, g_matProjInv);

	vector			vWorldPos;
	vWorldPos = mul(vViewPos, g_matViewInv);

	//Speculatr Compute
	vector	vLook = vWorldPos - g_vCamPosition;

	vector	vReflect = reflect(g_vLightDir, vWorldNormal);

	//vector	vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 3.f) ;

	//Out.vSpecular = g_vMtrlSpecular * vSpecular * tex2D(SpecularSampler, In.vTexUV);
	
	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector			vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector			vDepthInfo = tex2D(DepthSampler, In.vTexUV);
	float			fViewZ = vDepthInfo.y *g_fFarPlane;

	vector			vWorldNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	vector			vProjPos;

	//// 0 ~ -1, 1 ~ 1
	vProjPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;

	//// 0 ~ 1, 1 ~ -1
	vProjPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	
	vProjPos.z = vDepthInfo.x * fViewZ;

	vProjPos.w = fViewZ;

	vector			vViewPos;
	vViewPos = mul(vProjPos, g_matProjInv);

	vector			vWorldPos;
	vWorldPos = mul(vViewPos, g_matViewInv);

	vector			vLightDir = vWorldPos - g_vLightPos;

	float			fDistance = length(vLightDir);

	vector			vShade = max(dot(normalize(vLightDir) * -1.f, vWorldNormal), 0.f);

	float			fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);

	Out.vShade = (g_vLightDiffuse * g_vMtrlDiffuse) * (vShade + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

	Out.vShade.a = 1.f;

	//Speculatr Compute
	vector	vReflect = reflect(vLightDir, vWorldNormal);

	vector	vLook = vWorldPos - g_vCamPosition;

	vector	vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 3.f);//1.5f);

	Out.vSpecular = g_vMtrlSpecular * vSpecular * fAtt *  tex2D(SpecularSampler, In.vTexUV);
	//Out.vSpecular = tex2D(SpecularSampler, In.vTexUV);

	return Out;
}

technique Default_Device
{
	pass Directional
	{
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}
	pass Point
	{
		
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}
}