float4x4 g_matViewProj;
float fTime;

float4 vWaveFreq;
float4 vWaveSpeed;

float4 vWaveDirX;
float4 vWaveDirY;

float4 vWaveHeight;

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 vS = frac((input.tex.x*vWaveDirX + input.tex.y*vWaveDirY + fTime*vWaveSpeed)*vWaveFreq)*2.0f*3.1415926f;

    float4 vSin, vCos;
    sincos(vS, vSin, vCos);

    float4 newpos = float4(input.pos, 1.0f);
    newpos.y = newpos.y + dot(vSin, vWaveHeight);
    output.pos = mul(newpos, g_matViewProj);

    return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
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
