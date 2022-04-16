texture		g_BlendedTexture;

sampler BlendedSampler = sampler_state
{
	texture = g_BlendedTexture;
};


float2 fOffset = { 1/800.f, 1/600.f };

float2 PixelKernelH[13] =
{
	{ -6, 0 },{ -5, 0 },{ -4, 0 },{ -3, 0 },{ -2, 0 },{ -1, 0 },
	{ 0, 0 },
	{ 1, 0 },{ 2, 0 },{ 3, 0 },{ 4, 0 },{ 5, 0 },{ 6, 0 },
};

float2 PixelKernelV[13] =
{
	{ 0, -6 },{ 0, -5 },{ 0, -4 },{ 0, -3 },{ 0, -2 },{ 0, -1 },
	{ 0,  0 },
	{ 0,  1 },{ 0,  2 },{ 0,  3 },{ 0,  4 },{ 0,  5 },{ 0,  6 },
};

float BlurWeights[13] =
{
	0.002216,    0.008764,    0.026995,    0.064759,   0.120985,    0.176033,    
	0.199471,   
	0.176033,   0.120985,    0.064759,	  0.026995,    0.008764,    0.002216,

};


struct PS_IN
{
	float4	vPosition :POSITION; 
	float2	vTexUV : TEXCOORD;
};

struct PS_OUT
{
	vector	vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;


	float4 Color = tex2D(BlendedSampler, In.vTexUV);
	Color = pow(Color, 32);

	float4 Color2 = -0.5;
	//float4 Color2 = -0.84;

	for (int index = 0; index < 13; ++index) {
		Color2 += tex2D(BlendedSampler, In.vTexUV + (PixelKernelH[index] * fOffset)) * BlurWeights[index];
		Color2 += tex2D(BlendedSampler, In.vTexUV + (PixelKernelV[index] * fOffset)) * BlurWeights[index];
	}
	Color2 *= 0.48;

	float4 Color3 = tex2D(BlendedSampler, In.vTexUV);

	Out.vColor = Color + Color2 + Color3;

	return  Out;
}


technique Default_Device
{

	pass Default_Rendering
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0;

		ZWriteEnable = false;
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}


}