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
    float3 tangent : TEXCOORD2;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 texcoord : TEXCOORD0;
    float3 lightdir : TEXCOORD1;
    float3 eyedir : TEXCOORD2;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

    output.pos = mul(float4(input.pos, 1.0f), g_matWorldViewProj);
    output.texcoord = input.texcoord;

    float3 binormal = cross(input.tangent, input.normal);
    float3x3 rotation = float3x3(input.tangent, binormal, input.normal);

    float3 vLightDir = g_vLightPos - input.pos;
    output.lightdir = mul(rotation, vLightDir);

    float3 vEyeDir = g_vEyePos - input.pos;
    output.eyedir = mul(rotation, vEyeDir);

    return output;
}

float4 PSMainNormal(VS_OUTPUT input) : COLOR
{
    float3 lightdir = normalize(input.lightdir);

    float3 basecolor = tex2D(texBaseSampler, input.texcoord);

    float3 normal = tex2D(texNormalSampler, input.texcoord);
    normal = (normal - 0.5f) * 2.0f;
    float3 specular = dot(normal, lightdir);

    return float4(basecolor*specular, 1.0f);
}

float4 PSMainParallax(VS_OUTPUT input) : COLOR
{
    float3 lightdir = normalize(input.lightdir);
    float3 eyedir = normalize(input.eyedir);

    //float hsb = tex2D(texHeightSampler, input.texcoord).x * 0.04f - 0.02f;
    float hsb = tex2D(texHeightSampler, input.texcoord).x * 0.02f;
    float2 offsetuv = float2(eyedir.x, eyedir.y)*hsb + input.texcoord;
    float3 basecolor = tex2D(texBaseSampler, offsetuv);

    float3 normal = tex2D(texNormalSampler, offsetuv);
    normal = (normal - 0.5f) * 2.0f;
    float3 specular = dot(normal, lightdir);

    return float4(basecolor*specular, 1.0f);
}

float4 PSMainBaseTex(VS_OUTPUT input) : COLOR
{
    float3 basecolor = tex2D(texBaseSampler, input.texcoord);
    return float4(basecolor, 1.0f);
}

float4 PSMainNormalTex(VS_OUTPUT input) : COLOR
{
    float3 basecolor = tex2D(texNormalSampler, input.texcoord);
    return float4(basecolor, 1.0f);
}

float4 PSMainHeightTex(VS_OUTPUT input) : COLOR
{
    float3 basecolor = tex2D(texHeightSampler, input.texcoord);
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

technique Parallax
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMainParallax();
    }
}

technique BaseTex
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMainBaseTex();
    }
}

technique NormalTex
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMainNormalTex();
    }
}

technique HeightTex
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMainHeightTex();
    }
}
