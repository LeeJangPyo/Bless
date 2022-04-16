
matrix			g_matWorld, g_matView, g_matProj;
float			g_fFarPlane;

//RImLight
vector			g_vCameraPos;
float			g_fLimRightWidth;

texture			g_DiffuseTexture;
sampler	DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_NormalTexture;

sampler	NormalSampler = sampler_state
{
	texture = g_NormalTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_SpecularTexture;

sampler	SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


/////////////////////////////////////////////////////////////////////////////////
//DEFAULT
struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD;
};
struct VS_OUT
{
	float4		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vProjPos = Out.vPosition;

	return Out;
}



struct PS_IN
{
	float4		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vDiffuse : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
};

PS_OUT PS_MAIN(PS_IN In)
{

	PS_OUT		Out = (PS_OUT)0;

	// 지형을 이루는 특정 픽셀의 디퓨즈 머테리얼을 텍스쳐로부터 얻어온다.
	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	// -1 ~ 1
	//  0 ~ 1
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);

	return Out;
}

/////////////////////////////////////////////////////////////////////////////////
//NORMAPMAPPPING
struct VS_IN_NORMAL
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float3		vTangent : TANGENT;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT_NORMAL
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	vector		vBinormal : TEXCOORD2;
	vector		vTangent : TEXCOORD3;
	vector		vNormal : TEXCOORD4;
};


VS_OUT_NORMAL VS_MAIN_NORMAL(VS_IN_NORMAL In)
{
	VS_OUT_NORMAL	Out = (VS_OUT_NORMAL)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vProjPos = Out.vPosition;

	float3 vBinormal = cross(In.vNormal, In.vTangent);

	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vBinormal = normalize(mul(vector(vBinormal, 0.f), g_matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_matWorld));

	return Out;
}



struct PS_IN_NORMAL
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	vector		vBinormal : TEXCOORD2;
	vector		vTangent : TEXCOORD3;
	vector		vNormal : TEXCOORD4;
};

struct PS_OUT_NORMAL
{
	vector		vDiffuse : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
};

PS_OUT_NORMAL PS_MAIN_NORMAL(PS_IN_NORMAL In)
{
	PS_OUT_NORMAL		Out = (PS_OUT_NORMAL)0;

	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	vector vTangetNormal = tex2D(NormalSampler, In.vTexUV);
	vTangetNormal = normalize(vTangetNormal * 2 - 1);

	float4x4 matTangent = float4x4(In.vTangent, In.vBinormal, In.vNormal, float4(0.f, 0.f, 0.f, 1.f));
	//matTangent = transpose(matTangent);

	vector vWorldNormal = mul(vTangetNormal, matTangent);

	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, 0.f);

	//깊이값저장
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFarPlane, 0.f, 0.f);

	
	return Out;
}

/////////////////////////////////////////////////////////////////////////////////
//NORMAL+SPECULAR MAPPPING
struct VS_IN_NORMAL_SPECULAR
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float3		vTangent : TANGENT;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT_NORMAL_SPECULAR
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	vector		vBinormal : TEXCOORD2;
	vector		vTangent : TEXCOORD3;
	vector		vNormal : TEXCOORD4;
};


VS_OUT_NORMAL_SPECULAR VS_MAIN_NORMAL_SPECULAR(VS_IN_NORMAL_SPECULAR In)
{
	VS_OUT_NORMAL_SPECULAR		Out = (VS_OUT_NORMAL_SPECULAR)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vProjPos = Out.vPosition;

	float3 vBinormal = cross(In.vNormal, In.vTangent);

	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vBinormal = normalize(mul(vector(vBinormal, 0.f), g_matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_matWorld));

	return Out;
}

struct PS_IN_NORMAL_SPECULAR
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	vector		vBinormal : TEXCOORD2;
	vector		vTangent : TEXCOORD3;
	vector		vNormal : TEXCOORD4;
};

struct PS_OUT_NORMAL_SPECULAR
{
	vector		vDiffuse : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
	vector		vSpecular : COLOR3;
};

PS_OUT_NORMAL_SPECULAR PS_MAIN_NORMAL_SPECULAR(PS_IN_NORMAL_SPECULAR In)
{
	PS_OUT_NORMAL_SPECULAR		Out = (PS_OUT_NORMAL_SPECULAR)0;

	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	vector vTangetNormal = tex2D(NormalSampler, In.vTexUV);
	vTangetNormal = normalize(vTangetNormal * 2 - 1);

	float4x4 matTangent = float4x4(In.vTangent, In.vBinormal, In.vNormal, float4(0.f, 0.f, 0.f, 1.f));
	//matTangent = transpose(matTangent);

	vector vWorldNormal = mul(vTangetNormal, matTangent);

	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, 0.f);

	//깊이값저장
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFarPlane, 0.f, 0.f);

	//Specular
	vector vSpecularMap = tex2D(SpecularSampler, In.vTexUV);
	//Out.vSpecular = vector(1.f, 1.f, 1.f, vSpecularMap.b);
	Out.vSpecular = vector(vSpecularMap.r, vSpecularMap.r, vSpecularMap.r, vSpecularMap.b);


	return Out;
}

/////////////////////////////////////////////////////////////////////////////////
//NORMAL+SPECULAR+RIMLIGHT MAPPPING
struct VS_IN_NOR_SPEC_RIM
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float3		vTangent : TANGENT;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT_NOR_SPEC_RIM
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	vector		vBinormal : TEXCOORD2;
	vector		vTangent : TEXCOORD3;
	vector		vNormal : TEXCOORD4;
	vector		vWorldPos	: TEXCOORD5;
};


VS_OUT_NOR_SPEC_RIM VS_MAIN_NOR_SPEC_RIM(VS_IN_NOR_SPEC_RIM In)
{
	VS_OUT_NOR_SPEC_RIM		Out = (VS_OUT_NOR_SPEC_RIM)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vProjPos = Out.vPosition;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);

	float3 vBinormal = cross(In.vNormal, In.vTangent);

	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vBinormal = normalize(mul(vector(vBinormal, 0.f), g_matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_matWorld));

	return Out;
}

struct PS_IN_NOR_SPEC_RIM
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	vector		vBinormal : TEXCOORD2;
	vector		vTangent : TEXCOORD3;
	vector		vNormal : TEXCOORD4;
	vector		vWorldPos	: TEXCOORD5;
};

struct PS_OUT_NOR_SPEC_RIM
{
	vector		vDiffuse : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
	vector		vSpecular : COLOR3;
};

PS_OUT_NOR_SPEC_RIM PS_MAIN_NOR_SPEC_RIM(PS_IN_NOR_SPEC_RIM In)
{
	PS_OUT_NOR_SPEC_RIM		Out = (PS_OUT_NOR_SPEC_RIM)0;

	vector vTangetNormal = tex2D(NormalSampler, In.vTexUV);
	vTangetNormal = normalize(vTangetNormal * 2 - 1);

	float4x4 matTangent = float4x4(In.vTangent, In.vBinormal, In.vNormal, float4(0.f, 0.f, 0.f, 1.f));
	//matTangent = transpose(matTangent);

	vector vWorldNormal = mul(vTangetNormal, matTangent);
	vector vCameraDir = normalize(g_vCameraPos - In.vWorldPos);

	float RimLightColor = smoothstep(1.f - g_fLimRightWidth, 1.f,1.f - max(0,dot(vCameraDir, normalize(vWorldNormal))));
	//float RimLightColor = dot(vCameraDir, normalize(vWorldNormal)));

	Out.vNormal = vector(vWorldNormal.xyz * 0.5f + 0.5f, 0.f);

	//Depath
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFarPlane, 0.f, 0.f);

	//Specular
	vector vSpecularMap = tex2D(SpecularSampler, In.vTexUV);
	//Out.vSpecular = vector(1.f, 1.f, 1.f, vSpecularMap.b);
	Out.vSpecular = vector(vSpecularMap.r, vSpecularMap.r, vSpecularMap.r, vSpecularMap.b);

	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV) +RimLightColor;
	//Out.vDiffuse.r += RimLightColor;

	return Out;
}

/////////////////////////////////////////////////////////////////////////////////
//Shawdow_Mapping

struct VS_IN_SHADOW
{
	float3		vPosition : POSITION;

};

struct VS_OUT_SHADOW
{
	vector		vPosition : POSITION;
	vector		vProjPos : TEXCOORD0;

};


VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN_SHADOW In)
{
	VS_OUT_SHADOW		Out = (VS_OUT_SHADOW)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}


struct PS_IN_SHADOW
{
	vector		vPosition : POSITION;
	vector		vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
	vector		vShadow : COLOR0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	//깊이값저장
	Out.vShadow = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFarPlane, 0.f, 0.f);

	return Out;
}


/////////////////////////////////////////////////////////////////////////////////
//Technique
technique Default_Device
{
	pass DEFAULT//0
	{
		CullMode = CCW;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Collider	//1
	{
		CullMode = None;
		FillMode = WireFrame;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass NormalMapping	//2
	{
		CullMode = CCW;
		VertexShader = compile vs_3_0 VS_MAIN_NORMAL();
		PixelShader = compile ps_3_0 PS_MAIN_NORMAL();
	}

	pass Normal_SPECULAR_Mapping	//3
	{
		CullMode = CCW;
		VertexShader = compile vs_3_0 VS_MAIN_NORMAL_SPECULAR();
		PixelShader = compile ps_3_0 PS_MAIN_NORMAL_SPECULAR();
	}


	pass Shadow_Mapping	//4
	{
		CullMode = CCW;
		ZEnable = true;
		
		VertexShader = compile vs_3_0 VS_MAIN_SHADOW();
		PixelShader = compile ps_3_0 PS_MAIN_SHADOW();
	}

	pass Normal_SPECULAR_Mapping_RimLight	//5
	{
		CullMode = CCW;
		VertexShader = compile vs_3_0 VS_MAIN_NOR_SPEC_RIM();
		PixelShader = compile ps_3_0 PS_MAIN_NOR_SPEC_RIM();
	}
}
