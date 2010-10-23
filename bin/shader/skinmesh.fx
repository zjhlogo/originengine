float4x4 g_matWorldToModel;
float4x4 g_matWorldToProject;
float4x3 g_matBoneMatrix[50] : WORLDMATRIXARRAY;

float3 g_vLightPos;
float3 g_vEyePos;

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
	float3 eyeDir : TEXCOORD2;
};

float3 SkinMesh4(float4 posIn, int4 boneIndex, float4 boneWeight)
{
	float3 posOut[4];
	posOut[0] = mul(posIn, g_matBoneMatrix[boneIndex[0]]) * boneWeight[0];
	posOut[1] = mul(posIn, g_matBoneMatrix[boneIndex[1]]) * boneWeight[1];
	posOut[2] = mul(posIn, g_matBoneMatrix[boneIndex[2]]) * boneWeight[2];
	posOut[3] = mul(posIn, g_matBoneMatrix[boneIndex[3]]) * boneWeight[3];

	return (posOut[0] + posOut[1] + posOut[2] + posOut[3]);
}

float3 SkinMesh3(float3 posIn, int4 boneIndex, float4 boneWeight)
{
	float3 posOut[4];
	posOut[0] = mul(posIn, g_matBoneMatrix[boneIndex[0]]) * boneWeight[0];
	posOut[1] = mul(posIn, g_matBoneMatrix[boneIndex[1]]) * boneWeight[1];
	posOut[2] = mul(posIn, g_matBoneMatrix[boneIndex[2]]) * boneWeight[2];
	posOut[3] = mul(posIn, g_matBoneMatrix[boneIndex[3]]) * boneWeight[3];

	return (posOut[0] + posOut[1] + posOut[2] + posOut[3]);
}

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

	float3 skinnedPos = SkinMesh4(float4(input.pos, 1.0f), input.boneIndex, input.boneWeight);

	output.pos = mul(float4(skinnedPos, 1.0f), g_matWorldToProject);
	output.texcoord = input.texcoord;

	float3 skinnedNormal = SkinMesh3(input.normal, input.boneIndex, input.boneWeight);
	float3 normal = normalize(skinnedNormal);

	float3 skinnedTangent = SkinMesh3(input.tangent, input.boneIndex, input.boneWeight);
	float3 tangent = normalize(skinnedTangent);

	float3 binormal = cross(tangent, normal);
	float3x3 TangentToModel = float3x3(tangent, binormal, normal);

	float3 lightPos = mul(g_vLightPos, g_matWorldToModel);
	float3 vLightDir = lightPos - skinnedPos;
	output.lightDir = mul(TangentToModel, vLightDir);				// 这里相当于 mul(vLightDir, TangentToModel.Invert());

	float3 eyePos = mul(g_vEyePos, g_matWorldToModel);
	float3 vEyeDir = eyePos - skinnedPos;
	output.eyeDir = mul(TangentToModel, vEyeDir);					// 这里相当于 mul(vEyeDir, TangentToModel.Invert());

	return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);
	float3 eyeDir = normalize(input.eyeDir);
	float3 halfWayDir = normalize(lightDir + eyeDir);

	float4 diffuse = tex2D(sampleDiffuse, input.texcoord);
	float3 normal = tex2D(sampleNormal, input.texcoord);
	normal = (normal - 0.5f) * 2.0f;

	float diffuseFactor = saturate(dot(normal, lightDir));
	float specularFactor = pow(saturate(dot(normal, halfWayDir)), 8.0f);

	float3 finalColor = diffuse.xyz*diffuseFactor + float3(diffuse.w, diffuse.w, diffuse.w)*specularFactor;
	//finalColor = finalColor*0.8f + diffuse*0.2f;
	return float4(finalColor, 1.0f);
}

technique Normal
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}
