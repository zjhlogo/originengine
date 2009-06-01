float4x4 g_matViewProj;
texture g_texBase;

sampler texBaseSampler = 
sampler_state
{
    Texture = <g_texBase>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 texcoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 texcoord : TEXCOORD0;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

    output.pos = mul(float4(input.pos, 1.0f), g_matViewProj);
    output.texcoord = input.texcoord;
    return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
    float4 color = tex2D(texBaseSampler, input.texcoord);
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

technique Normal
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMain();
    }
}
