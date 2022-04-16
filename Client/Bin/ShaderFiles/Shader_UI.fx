matrix		g_matWorld, g_matView, g_matProj;

float		g_fRadio;
float		g_fTimeAcc;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
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

///////////////////////////////////////////////////////////////////////////////////
// Default
struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

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
	float4	vPosition :POSITION;
	float2	vTexUV : TEXCOORD0;

};

struct PS_OUT
{
	vector	vColor : COLOR;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vColor;

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bar
struct VS_IN_BAR
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT_BAR
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float2		vShadeTexUV : TEXCOORD1;
};

VS_OUT_BAR	VS_MAIN_BAR(VS_IN_BAR In)
{
	VS_OUT_BAR		Out = (VS_OUT_BAR)0;
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vShadeTexUV = In.vTexUV + float2(g_fTimeAcc, 0);

	return Out;
}


struct PS_IN_BAR
{
	float4	vPosition :POSITION; // 모든 위치 변환은 종료, 추갖거으로 위치변환 불가
	float2	vTexUV : TEXCOORD0;
	float2	vShadeTexUV : TEXCOORD1;
};

struct PS_OUT_BAR
{
	vector	vColor : COLOR;
};


PS_OUT_BAR PS_MAIN_BAR(PS_IN_BAR In)
{
	PS_OUT_BAR	Out = (PS_OUT_BAR)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV) * tex2D(ShadeSampler, In.vShadeTexUV);// +vMask;

	Out.vColor = vColor;

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	BarMask
struct VS_IN_BARMASK
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT_BARMASK
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT_BARMASK	VS_MAIN_BARMASK(VS_IN_BARMASK In)
{
	VS_OUT_BARMASK		Out = (VS_OUT_BARMASK)0;
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN_BARMASK
{
	float4	vPosition :POSITION; // 모든 위치 변환은 종료, 추갖거으로 위치변환 불가
	float2	vTexUV : TEXCOORD0;

};

struct PS_OUT_BARMASK
{
	vector	vColor : COLOR;
};


PS_OUT_BARMASK PS_MAIN_BARMASK(PS_IN_BARMASK In)
{
	PS_OUT_BARMASK	Out = (PS_OUT_BARMASK)0;


	vector vColor = tex2D(BaseSampler, In.vTexUV);

	if (In.vTexUV.x < g_fRadio)
		vColor.a = 0.f;

	Out.vColor = vColor;

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	IconMask


struct VS_IN_ICONMASK
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT_ICONMASK
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT_ICONMASK	VS_MAIN_ICONMASK(VS_IN_ICONMASK In)
{
	VS_OUT_ICONMASK		Out = (VS_OUT_ICONMASK)0;
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN_ICONMASK
{
	float4	vPosition :POSITION; // 모든 위치 변환은 종료, 추갖거으로 위치변환 불가
	float2	vTexUV : TEXCOORD0;

};

struct PS_OUT_ICONMASK
{
	vector	vColor : COLOR;
};


PS_OUT_ICONMASK PS_MAIN_ICONMASK(PS_IN_ICONMASK In)
{
	PS_OUT_ICONMASK	Out = (PS_OUT_ICONMASK)0;


	vector vColor = tex2D(BaseSampler, In.vTexUV);

	vColor.a = 0.f;

	Out.vColor = vColor;

	return Out;
}

PS_OUT_ICONMASK PS_MAIN_COOLDOWN(PS_IN_ICONMASK In)
{
	PS_OUT_ICONMASK	Out = (PS_OUT_ICONMASK)0;


	vector vColor = tex2D(BaseSampler, In.vTexUV);

	if (In.vTexUV.x < g_fRadio)
		vColor.a = 0.f;

	Out.vColor = vColor;

	return Out;
}



technique Default_Device
{
	//0
	pass Default_Rendering
	{
		CullMode = None;
		ZENABLE = false;
		//blend
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		/*ALPHATESTENABLE = TRUE;
		ALPHAFUNC = GREATER;
		ALPHAREF = 0;*/

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	
	//1
	pass Bar_Rendering
	{
		CullMode = None;
		ZENABLE = false;
		//ALPHATESTENABLE = TRUE;
		//ALPHAFUNC = GREATER;
		//ALPHAREF = 0;

		//AlphaBlendEnable = true;
		//SrcBlend = SRCALPHA;
		//DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_BAR();
		PixelShader = compile ps_3_0 PS_MAIN_BAR();
	}

	//2
	pass BarMask_Rendering
	{
		CullMode = None;
		ZENABLE = false;
		//Test
		ALPHATESTENABLE = TRUE;
		ALPHAFUNC = GREATER;
		ALPHAREF = 0;
		/*AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;*/

		VertexShader = compile vs_3_0 VS_MAIN_BARMASK();
		PixelShader = compile ps_3_0 PS_MAIN_BARMASK();
	}

	pass IconMask_Rendering
	{
		CullMode = None;

		//Test
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_ICONMASK();
		PixelShader = compile ps_3_0 PS_MAIN_ICONMASK();
	}

	pass CoolDown_Rendering
	{
		CullMode = None;

		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_ICONMASK();
		PixelShader = compile ps_3_0 PS_MAIN_COOLDOWN();
	}
}