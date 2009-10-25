float4x4 g_matWorldViewProj;
float4x4 g_matBoneMatrix[61] : WORLDMATRIXARRAY;

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
	int4 boneindex : BLENDINDICES;
	float4 boneweight : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 texcoord : TEXCOORD0;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

	float4 posin = float4(input.pos, 1.0f);
	float4 posout[4];
	posout[0] = mul(posin, g_matBoneMatrix[input.boneindex[0]]) * input.boneweight[0];
	posout[1] = mul(posin, g_matBoneMatrix[input.boneindex[1]]) * input.boneweight[1];
	posout[2] = mul(posin, g_matBoneMatrix[input.boneindex[2]]) * input.boneweight[2];
	posout[3] = mul(posin, g_matBoneMatrix[input.boneindex[3]]) * input.boneweight[3];

	float4 skinnedpos = posout[0] + posout[1] + posout[2] + posout[3];
    output.pos = mul(skinnedpos, g_matWorldViewProj);
	output.texcoord = input.texcoord;

    return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
    float4 color = tex2D(texBaseSampler, input.texcoord);
    return color;
}

technique Normal
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMain();
    }
}
