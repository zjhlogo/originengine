float4x4 g_matWorldViewProj;
float3 g_vLightPos;
texture g_texBase;
texture g_texNormal;

sampler texBaseSampler =
sampler_state
{
    Texture = <g_texBase>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler texNormalSampler =
sampler_state
{
	Texture = <g_texNormal>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 tangent : TEXCOORD2;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 texcoord : TEXCOORD0;
	float3 lightdir : TEXCOORD1;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

    output.pos = mul(float4(input.pos, 1.0f), g_matWorldViewProj);
    output.texcoord = input.texcoord;

	float3 vLightDir = g_vLightPos - input.pos;

	float3 binormal = cross(input.tangent, input.normal);
	float3x3 rotation = float3x3(input.tangent, binormal, input.normal);
	output.lightdir = mul(rotation, vLightDir);

	return output;
}

float4 PSMainNormal(VS_OUTPUT input) : COLOR
{
	float3 lightdir = normalize(input.lightdir);

	float3 normal = tex2D(texNormalSampler, input.texcoord);
	normal = (normal - 0.5f) * 2.0f;

	float3 specular = dot(normal, lightdir);
	float3 basecolor = tex2D(texBaseSampler, input.texcoord);

	return float4(basecolor, 1.0f)*0.8f+float4(specular, 1.0f)*0.2f;
}

float4 PSMainBase(VS_OUTPUT input) : COLOR
{
	float3 basecolor = tex2D(texBaseSampler, input.texcoord);
	return float4(basecolor, 1.0f);
}

float4 PSMainNormalMap(VS_OUTPUT input) : COLOR
{
	float3 basecolor = tex2D(texNormalSampler, input.texcoord);
	return float4(basecolor, 1.0f);
}

technique Normal
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMainNormal();
    }
}

technique Base
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMainBase();
    }
}

technique NormalMap
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMainNormalMap();
    }
}
