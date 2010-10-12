float4x4 g_matWorldViewProj;
float3 g_vLightPos;
float3 g_vEyePos;

texture g_texDiffuse;
texture g_texNormalHeight;

sampler sampleDiffuse =
sampler_state
{
	Texture = <g_texDiffuse>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler sampleNormalHeight =
sampler_state
{
	Texture = <g_texNormalHeight>;
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

	float4 normalHeight = tex2D(sampleNormalHeight, input.texcoord);
	float3 normal = (normalHeight.xyz - 0.5f) * 2.0f;

	float3 diffuseFactor = saturate(dot(normal, lightDir));

	float3 finalColor = diffuse*diffuseFactor;
	return float4(finalColor, 1.0f);
}

float4 PSMainParallaxMap(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);
	float3 eyeDir = normalize(input.eyeDir);

	float hsb = tex2D(sampleNormalHeight, input.texcoord).w * 0.05f;
	float2 offsetuv = float2(eyeDir.x, eyeDir.y)*hsb + input.texcoord;
	float4 normalHeight = tex2D(sampleNormalHeight, offsetuv);
	float3 normal = (normalHeight.xyz - 0.5f) * 2.0f;

	float3 diffuse = tex2D(sampleDiffuse, offsetuv);

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
	float3 normal = tex2D(sampleNormalHeight, input.texcoord);
	return float4(normal, 1.0f);
}

float4 PSMainHeightMapTexture(VS_OUTPUT input) : COLOR
{
	float4 heightMap = tex2D(sampleNormalHeight, input.texcoord);
	return float4(heightMap.w, heightMap.w, heightMap.w, 1.0f);
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
