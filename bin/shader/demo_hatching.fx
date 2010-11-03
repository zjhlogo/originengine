float4x4 g_matWorldToModel;
float4x4 g_matWorldToProject;
float3 g_vLightPos;

texture g_texDiffuse;
texture g_texHatchLevel;

sampler sampleDiffuse =
sampler_state
{
	Texture = <g_texDiffuse>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler sampleHatchingLevel =
sampler_state
{
	Texture = <g_texHatchLevel>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 lightDir : TEXCOORD1;
	float3 normal : TEXCOORD2;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(float4(input.pos, 1.0f), g_matWorldToProject);
	output.texcoord = input.texcoord;

	float3 lightPos = mul(g_vLightPos, g_matWorldToModel);
	output.lightDir = lightPos - input.pos;

	output.normal = input.normal;
	return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);
	float3 normal = normalize(input.normal);

	float diffuseFactor = saturate(dot(normal, lightDir));
	float4 hatchingFactor = tex2D(sampleHatchingLevel, float2(diffuseFactor, 0.5f));

	float4 diffuse = tex2D(sampleDiffuse, input.texcoord);
	float4 final = diffuse * hatchingFactor;

	float color = final.r + final.g + final.b;
	return float4(color, color, color, 1.0f);
}

technique Normal
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMain();
	}
}
