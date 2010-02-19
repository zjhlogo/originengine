texture g_texDiffuse;

sampler sampleDiffuse =
sampler_state
{
	Texture = <g_texDiffuse>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD0;
};

float4 PSMain(VS_OUTPUT input) : COLOR
{
	return tex2D(sampleDiffuse, input.texcoord);
}

technique Normal
{
	pass p0
	{
		PixelShader = compile ps_2_0 PSMain();
	}
}
