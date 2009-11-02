float4x4 g_matWorldViewProj;
float4x3 g_matBoneMatrix[50] : WORLDMATRIXARRAY;

float3 g_vLightPos = {-300.0f, 0.0f, -300.0f};

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
	int4 boneindex : BLENDINDICES;
	float4 boneweight : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 texcoord : TEXCOORD0;
    float3 lightdir : TEXCOORD1;
};

float3 SkinMesh4(float4 posin, int4 boneindex, float4 boneweight)
{
	float3 posout[4];
	posout[0] = mul(posin, g_matBoneMatrix[boneindex[0]]) * boneweight[0];
	posout[1] = mul(posin, g_matBoneMatrix[boneindex[1]]) * boneweight[1];
	posout[2] = mul(posin, g_matBoneMatrix[boneindex[2]]) * boneweight[2];
	posout[3] = mul(posin, g_matBoneMatrix[boneindex[3]]) * boneweight[3];

	return (posout[0] + posout[1] + posout[2] + posout[3]);
}

float3 SkinMesh3(float3 posin, int4 boneindex, float4 boneweight)
{
	float3 posout[4];
	posout[0] = mul(posin, g_matBoneMatrix[boneindex[0]]) * boneweight[0];
	posout[1] = mul(posin, g_matBoneMatrix[boneindex[1]]) * boneweight[1];
	posout[2] = mul(posin, g_matBoneMatrix[boneindex[2]]) * boneweight[2];
	posout[3] = mul(posin, g_matBoneMatrix[boneindex[3]]) * boneweight[3];

	return (posout[0] + posout[1] + posout[2] + posout[3]);
}

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

	float3 skinnedpos = SkinMesh4(float4(input.pos, 1.0f), input.boneindex, input.boneweight);

    output.pos = mul(float4(skinnedpos, 1.0f), g_matWorldViewProj);
	output.texcoord = input.texcoord;

	float3 skinnednormal = SkinMesh3(input.normal, input.boneindex, input.boneweight);
	float3 normal = normalize(skinnednormal);
	//float3 normal = input.normal;

	float3 skinnedtangent = SkinMesh3(input.tangent, input.boneindex, input.boneweight);
	float3 tangent = normalize(skinnedtangent);
	//float3 tangent = input.tangent;

    float3 binormal = cross(tangent, normal);
    float3x3 rotation = float3x3(tangent, binormal, normal);

    float3 vLightDir = g_vLightPos - skinnedpos;
    output.lightdir = mul(rotation, vLightDir);

    return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
    float3 lightdir = normalize(input.lightdir);

    float3 basecolor = tex2D(texBaseSampler, input.texcoord);

    float3 normal = tex2D(texNormalSampler, input.texcoord);
    normal = (normal - 0.5f) * 2.0f;
    float3 specular = dot(normal, lightdir);

    return float4(basecolor*specular, 1.0f);

	//float3 basecolor = tex2D(texBaseSampler, input.texcoord);
	//return float4(basecolor, 1.0f);
}

technique Normal
{
    pass p0
    {
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMain();
    }
}
