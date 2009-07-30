float4x4 g_matWorldViewProj;
float3 g_vLightPos;
float3 g_vEyePos;

texture g_texBase;
texture g_texNormal;
texture g_texHeight;

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

sampler texHeightSampler =
sampler_state
{
    Texture = <g_texHeight>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 texcoord : TEXCOORD0;
    float3 lightdir : TEXCOORD1;
    float3 normal : TEXCOORD2;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

    output.pos = mul(float4(input.pos, 1.0f), g_matWorldViewProj);
    output.texcoord = input.texcoord;
    output.lightdir = g_vLightPos - input.pos;
    output.normal = input.normal;

    return output;
}

float4 PSMainNormal(VS_OUTPUT input) : COLOR
{
    float3 lightdir = normalize(input.lightdir);

    float3 basecolor = tex2D(texBaseSampler, input.texcoord);
    float3 specular = dot(input.normal, lightdir);

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
