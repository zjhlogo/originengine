float4x4 g_matWorldToModel;
float4x4 g_matWorldToProject;
float3 g_vLightPos;

texture g_texDiffuse;
texture g_texNormal;

sampler sampleDiffuse =
sampler_state
{
	Texture = <g_texDiffuse>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler sampleNormal =
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
	float3 normal : NORMAL;
	float3 tangent : TEXCOORD1;
	int4 boneIndex : BLENDINDICES;
	float4 boneWeight : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 lightDir : TEXCOORD1;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(float4(input.pos, 1.0f), g_matWorldToProject);
	output.texcoord = input.texcoord;

	float3 binormal = cross(input.tangent, input.normal);
	float3x3 TangentToModel = float3x3(input.tangent, binormal, input.normal);

	float3 lightPos = mul(g_vLightPos, g_matWorldToModel);
	float3 vLightDir = lightPos - input.pos;
	output.lightDir = mul(TangentToModel, vLightDir);				// 这里相当于 mul(vLightDir, TangentToModel.Invert());

	return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);

	float3 diffuse = tex2D(sampleDiffuse, input.texcoord);
	float3 normal = tex2D(sampleNormal, input.texcoord);
	normal = (normal - 0.5f) * 2.0f;

	float diffuseFactor = saturate(dot(normal, lightDir));
	return float4(diffuse*diffuseFactor, 1.0f);
}

technique Normal
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMain();
	}
}
