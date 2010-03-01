float4x4 g_matWorldViewProj;
float3 g_vLightPos;
float3 g_vEyePos;

texture g_texDiffuse;
texture g_texNormal;
texture g_texHeightMap;

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

sampler sampleHeightMap =
sampler_state
{
	Texture = <g_texHeightMap>;
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
	float3 lightDir : TEXCOORD1;
	float3 eyeDir : TEXCOORD2;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(float4(input.pos, 1.0f), g_matWorldViewProj);
	output.texcoord = input.texcoord;

	float3 binormal = cross(input.tangent, input.normal);
	float3x3 rotation = float3x3(input.tangent, binormal, input.normal);

	float3 vLightDir = g_vLightPos - input.pos;
	output.lightDir = mul(rotation, vLightDir);

	float3 vEyeDir = g_vEyePos - input.pos;
	output.eyeDir = mul(rotation, vEyeDir);

	return output;
}

float4 PSMainNormalMap(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);

	float3 diffuse = tex2D(sampleDiffuse, input.texcoord);

	float3 normal = tex2D(sampleNormal, input.texcoord);
	normal = (normal - 0.5f) * 2.0f;

	float3 diffuseFactor = saturate(dot(normal, lightDir));

	float3 finalColor = diffuse*diffuseFactor;
	return float4(finalColor, 1.0f);
}

float4 PSMainParallaxMap(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);
	float3 eyeDir = normalize(input.eyeDir);

	//float hsb = tex2D(sampleHeightMap, input.texcoord).x * 0.04f - 0.02f;
	float hsb = tex2D(sampleHeightMap, input.texcoord).x * 0.02f;
	float2 offsetuv = float2(eyeDir.x, eyeDir.y)*hsb + input.texcoord;

	float3 diffuse = tex2D(sampleDiffuse, offsetuv);

	float3 normal = tex2D(sampleNormal, offsetuv);
	normal = (normal - 0.5f) * 2.0f;

	float3 diffuseFactor = saturate(dot(normal, lightDir));

	float3 finalColor = diffuse*diffuseFactor;
	return float4(finalColor, 1.0f);
}

float4 PSMainDiffuseTexture(VS_OUTPUT input) : COLOR
{
	return tex2D(sampleDiffuse, input.texcoord);
}

float4 PSMainNormalTexture(VS_OUTPUT input) : COLOR
{
	return tex2D(sampleNormal, input.texcoord);
}

float4 PSMainHeightMapTexture(VS_OUTPUT input) : COLOR
{
	return tex2D(sampleHeightMap, input.texcoord);
}

technique NormalMap
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMainNormalMap();
	}
}

technique ParallaxMap
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMainParallaxMap();
	}
}

technique DiffuseTexture
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMainDiffuseTexture();
	}
}

technique NormalTexture
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMainNormalTexture();
	}
}

technique HeightMapTexture
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMainHeightMapTexture();
	}
}