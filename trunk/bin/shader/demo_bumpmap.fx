float4x4 g_matWorldToModel;
float4x4 g_matWorldToProject;
float3 g_vLightPos;
float3 g_vEyePos;

texture g_texDiffuse;
texture g_texNormal;
texture g_texConeMap;

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

sampler sampleConeMap =
sampler_state
{
	Texture = <g_texConeMap>;
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

	output.pos = mul(float4(input.pos, 1.0f), g_matWorldToProject);
	output.texcoord = input.texcoord;

	float3 binormal = cross(input.tangent, input.normal);
	float3x3 TangentToModel = float3x3(input.tangent, binormal, input.normal);

	float3 lightPos = mul(g_vLightPos, g_matWorldToModel);
	float3 vLightDir = lightPos - input.pos;
	output.lightDir = mul(TangentToModel, vLightDir);			// 这里相当于 mul(vLightDir, TangentToModel.Invert());

	float3 eyePos = mul(g_vEyePos, g_matWorldToModel);
	float3 vEyeDir = eyePos - input.pos;
	output.eyeDir = mul(TangentToModel, vEyeDir);				// 这里相当于 mul(vEyeDir, TangentToModel.Invert());

	return output;
}

float4 PSMainNormalMap(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);
	float3 eyeDir = normalize(input.eyeDir);
	float3 halfWayDir = normalize(lightDir + eyeDir);

	float3 diffuse = tex2D(sampleDiffuse, input.texcoord);

	float4 normalHeight = tex2D(sampleNormal, input.texcoord);
	float3 normal = (normalHeight.xyz - 0.5f) * 2.0f;

	float3 diffuseFactor = saturate(dot(normal, lightDir));
	float specularFactor = pow(saturate(dot(normal, halfWayDir)), 32.0f);

	float3 finalColor = diffuse*diffuseFactor + float3(0.5f, 0.5f, 0.5f)*specularFactor;
	return float4(finalColor, 1.0f);
}

float4 PSMainParallaxMap(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);
	float3 eyeDir = normalize(input.eyeDir);
	float3 halfWayDir = normalize(lightDir + eyeDir);

	float hsb = tex2D(sampleNormal, input.texcoord).w * 0.04f - 0.02f;
	float2 offsetuv = float2(eyeDir.x, -eyeDir.y)*hsb + input.texcoord;
	float4 normalHeight = tex2D(sampleNormal, offsetuv);
	float3 normal = (normalHeight.xyz - 0.5f) * 2.0f;

	float3 diffuse = tex2D(sampleDiffuse, offsetuv);

	float3 diffuseFactor = saturate(dot(normal, lightDir));
	float specularFactor = pow(saturate(dot(normal, halfWayDir)), 32.0f);

	float3 finalColor = diffuse*diffuseFactor + float3(0.5f, 0.5f, 0.5f)*specularFactor;
	return float4(finalColor, 1.0f);
}

float intersect_cone_fixed(float2 dp, float3 ds)
{
	// over-relaxation parameter
	const float w = 1.0f;
	// the "not Z" component of the direction vector
	// (requires that the vector ds was normalized!)
	float iz = sqrt(1.0f-ds.z*ds.z);
	// my starting location (is at z=1,
	// and moving down so I don't have
	// to invert height maps)
	// texture lookup
	float4 t = {0.0f, 0.0f, 0.0f, 0.0f};
	// scaling distance along vector ds
	float sc = 0.0f;

	// the ds.z component is positive!
	// (headed the wrong way, since
	// I'm using heightmaps)

	for (int i = 10; i > 0; --i)
	{
		// find the new location and height
		t = tex2D(sampleConeMap, dp+ds.xy*sc);

		// right, I need to take one step.
		// I use the current height above the texture,
		// and the information about the cone-ratio
		// to size a single step.  So it is fast and
		// precise!  (like a coneified version of
		// "space leaping", but adapted from voxels)
		sc += (1.0f-ds.z*sc-t.r) / (iz/(t.g*t.g)-ds.z);
	}

	// return the vector length needed to hit the height-map
	return (sc);
}

float4 PSMainConeMap(VS_OUTPUT input) : COLOR
{
	float3 lightDir = normalize(input.lightDir);
	float3 eyeDir = normalize(input.eyeDir);
	float3 halfWayDir = normalize(lightDir + eyeDir);

	float3 negEyeDir = -eyeDir;
	negEyeDir = normalize(negEyeDir);
	float len = intersect_cone_fixed(input.texcoord, negEyeDir);
	float2 newTexcoord = input.texcoord+float2(negEyeDir.x, negEyeDir.y)*len;

	float3 diffuse = tex2D(sampleDiffuse, newTexcoord);

	float4 normalHeight = tex2D(sampleNormal, newTexcoord);
	float3 normal = (normalHeight.xyz - 0.5f) * 2.0f;

	float3 diffuseFactor = saturate(dot(normal, lightDir));
	float specularFactor = pow(saturate(dot(normal, halfWayDir)), 32.0f);

	float3 finalColor = diffuse*diffuseFactor + float3(0.5f, 0.5f, 0.5f)*specularFactor;
	return float4(finalColor, 1.0f);
}

float4 PSMainDiffuseTexture(VS_OUTPUT input) : COLOR
{
	return tex2D(sampleDiffuse, input.texcoord);
}

float4 PSMainNormalTexture(VS_OUTPUT input) : COLOR
{
	float3 normal = tex2D(sampleNormal, input.texcoord);
	return float4(normal, 1.0f);
}

float4 PSMainHeightMapTexture(VS_OUTPUT input) : COLOR
{
	float4 heightMap = tex2D(sampleNormal, input.texcoord);
	return float4(heightMap.w, heightMap.w, heightMap.w, 1.0f);
}

technique NormalMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMainNormalMap();
	}
}

technique ParallaxMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMainParallaxMap();
	}
}

technique ConeMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMainConeMap();
	}
}

technique DiffuseTexture
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMainDiffuseTexture();
	}
}

technique NormalTexture
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMainNormalTexture();
	}
}

technique HeightMapTexture
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMainHeightMapTexture();
	}
}
