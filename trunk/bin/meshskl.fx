float4x4 g_matWorldViewProj;
float4x4 g_matBoneMatrix[64] : WORLDMATRIXARRAY;

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
	float4 boneindex : BLENDINDICES;
	float4 boneweight : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 texcoord : TEXCOORD0;
};

float4 SkinnedPosition(const float4 pos, const float4 weights, const float4 indices)
{
	float4 posout[4];

	posout[0] = mul(pos, g_matBoneMatrix[indices[0]]) * weights[0];
	posout[1] = mul(pos, g_matBoneMatrix[indices[1]]) * weights[1];
	posout[2] = mul(pos, g_matBoneMatrix[indices[2]]) * weights[2];
	posout[3] = mul(pos, g_matBoneMatrix[indices[3]]) * weights[3];

	return (posout[0]+posout[1]+posout[2]+posout[3]);
}

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

	float4 skinnedpos = SkinnedPosition(float4(input.pos, 1.0f), input.boneweight, input.boneindex);
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
