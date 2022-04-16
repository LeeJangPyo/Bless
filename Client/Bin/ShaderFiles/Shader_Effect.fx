matrix		g_matWorld, g_matView, g_matProj;

int			g_iWidth;		//아틀라스이미지 x축 개수
int			g_iHeight;		//아틀라스이미지 y축 개수
int			g_iAnimationX;	//현재 애니메이션 인덱스 x
int			g_iAnimationY;	//현재 애니메이션 인덱스 y

//float		g_fBrightness;

float		g_fTimeAcc;
float		g_fUSpeed;
float		g_fVSpeed;

vector		g_vAlpha;
vector		g_vMaterial;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_AlphaTexture;
sampler AlhpaSampler = sampler_state
{
	texture = g_AlphaTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_NoiseTexture;
sampler NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;
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

	//vector vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vector(0.f, 0.f, 0.f, 1.f);

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Alpha
struct VS_IN_ALPHA
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT_ALPHA
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT_ALPHA	VS_MAIN_ALPHA(VS_IN_ALPHA In)
{
	VS_OUT_ALPHA		Out = (VS_OUT_ALPHA)0;
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	Out.vTexUV = In.vTexUV+ float2(g_fTimeAcc* g_fUSpeed, g_fTimeAcc* g_fVSpeed);

	return Out;
}

struct PS_IN_ALPHA
{
	float4	vPosition :POSITION; 
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT_ALPHA
{
	vector	vColor : COLOR;
};


PS_OUT_ALPHA PS_MAIN_ALPHA(PS_IN_ALPHA In)
{
	PS_OUT_ALPHA	Out = (PS_OUT_ALPHA)0;

	float2 UV;
	UV.x = In.vTexUV.x / g_iWidth + g_iAnimationX* 1.f/ g_iWidth;
	UV.y = In.vTexUV.y / g_iHeight + g_iAnimationY* 1.f / g_iHeight;

	vector vColor = tex2D(AlhpaSampler, UV); //In.vTexUV); // * tex2D(ShadeSampler, In.vShadeTexUV);// +vMask;

	Out.vColor = vColor * g_vMaterial * g_vAlpha;
	
	return Out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	NONE_ALPHA
PS_OUT_ALPHA PS_MAIN_NONEALPHA(PS_IN_ALPHA In)
{
	PS_OUT_ALPHA	Out = (PS_OUT_ALPHA)0;

	float2 UV;
	UV.x = In.vTexUV.x / g_iWidth + g_iAnimationX* 1.f / g_iWidth;
	UV.y = In.vTexUV.y / g_iHeight + g_iAnimationY* 1.f / g_iHeight;

	vector vColor = tex2D(AlhpaSampler, UV); //In.vTexUV); // * tex2D(ShadeSampler, In.vShadeTexUV);// +vMask;

											 //if (UV.x < g_fGradation || UV.y < g_fGradation)
											 //	Out.vColor.a = 0.f;
											 //else

	vColor.a = 0.f;
	vColor.a = max(vColor.a, vColor.r);
	vColor.a = max(vColor.a, vColor.g);
	vColor.a = max(vColor.a, vColor.b);

	Out.vColor = vColor * g_vMaterial * g_vAlpha;

	return Out;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Alpha + Texture
struct VS_IN_TWOTEXTURE
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

};

struct VS_OUT_TWOTEXTURE
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float2		vAlhpaUV: TEXCOORD1;
};

VS_OUT_TWOTEXTURE	VS_MAIN_TWOTEXTURE(VS_IN_TWOTEXTURE In)
{
	VS_OUT_TWOTEXTURE		Out = (VS_OUT_TWOTEXTURE)0;
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vAlhpaUV = In.vTexUV + float2(g_fTimeAcc* g_fUSpeed, g_fTimeAcc* g_fVSpeed);

	return Out;
}

struct PS_IN_TWOTEXTURE
{
	float4	vPosition :POSITION; // 모든 위치 변환은 종료, 추갖거으로 위치변환 불가
	float2	vTexUV : TEXCOORD0;
	float2	vAlhpaUV: TEXCOORD1;

};

struct PS_OUT_TWOTEXTURE
{
	vector	vColor : COLOR;
};


PS_OUT_TWOTEXTURE PS_MAIN_TWOTEXTURE(PS_IN_TWOTEXTURE In)
{
	PS_OUT_TWOTEXTURE	Out = (PS_OUT_TWOTEXTURE)0;



	float2 UV;
	UV.x = In.vTexUV.x / g_iWidth + g_iAnimationX* 1.f / g_iWidth;
	UV.y = In.vTexUV.y / g_iHeight + g_iAnimationY* 1.f / g_iHeight;

	 

	vector vColor;
	vector vAlhpa = tex2D(AlhpaSampler, UV); //In.vTexUV); // * tex2D(ShadeSampler, In.vShadeTexUV);// +vMask;
	vColor = vAlhpa * tex2D(BaseSampler, In.vAlhpaUV);
	vColor.a *= vAlhpa.r;
	Out.vColor = vColor * g_vMaterial *g_vAlpha;

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Effect_PARTICLE
struct VS_IN_PARTICLE
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_PARTICLE
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT_PARTICLE	VS_MAIN_PARTICLE(VS_IN_PARTICLE In)
{
	VS_OUT_PARTICLE		Out = (VS_OUT_PARTICLE)0;
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN_PARTICLE
{
	float4	vPosition :POSITION; // 모든 위치 변환은 종료, 추갖거으로 위치변환 불가
	float2	vTexUV : TEXCOORD0;

};

struct PS_OUT_PARTICLE
{
	vector	vColor : COLOR;
};


PS_OUT_PARTICLE PS_MAIN_PARTICLE(PS_IN_PARTICLE In)
{
	PS_OUT_PARTICLE	Out = (PS_OUT_PARTICLE)0;

	vector vColor;
	vColor = tex2D(BaseSampler, In.vTexUV);// +vMask;

	Out.vColor = vColor * g_vAlpha;

	return Out;
}

technique Default_Device
{
	//0
	pass Default_Rendering
	{
		CullMode = None;
		FillMode = WireFrame;
//		ZENABLE = false;
		//blend
		//AlphaBlendEnable = true;
		//SrcBlend = SRCALPHA;
		//DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	
	//1
	pass ALPHA_Rendering
	{
		CullMode = None;
	//	ZENABLE = false;
		ALPHATESTENABLE = TRUE;
		ALPHAFUNC = GREATER;
		ALPHAREF = 32;

		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_ALPHA();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHA();
	}


	//2
	pass TWOTEXTURE_Rendering
	{
		CullMode = None;
		//ZENABLE = false;
		//Test
		ALPHATESTENABLE = TRUE;
		ALPHAFUNC = GREATER;
		ALPHAREF = 32;

		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_TWOTEXTURE();
		PixelShader = compile ps_3_0 PS_MAIN_TWOTEXTURE();
	}
	//3
	pass NONEALPHA_Rendering
	{
		CullMode = None;
		//	ZENABLE = false;
		ALPHATESTENABLE = TRUE;
		ALPHAFUNC = GREATER;
		ALPHAREF = 32;

		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_ALPHA();
		PixelShader = compile ps_3_0 PS_MAIN_NONEALPHA();
	}

	//4
	pass Particle_Rendering
	{
		CullMode = None;
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_PARTICLE();
		PixelShader = compile ps_3_0 PS_MAIN_PARTICLE();
	}

}