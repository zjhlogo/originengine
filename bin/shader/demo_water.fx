float4x4 g_matWorldToProject;
float fTime;

float4 vWaveFreq;
float4 vWaveSpeed;

float4 vWaveDirX;
float4 vWaveDirY;

float4 vWaveHeight;

float3 g_vLightDir = {-1.0f, 1.0f, 1.0f};
float3 g_vEyePos;

struct VS_INPUT
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float3 eyedir : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 vRadius = frac((input.tex.x*vWaveDirX + input.tex.y*vWaveDirY + fTime*vWaveSpeed)*vWaveFreq)*2.0f*3.1415926f;

	float4 vSin;
	float4 vCos;
	sincos(vRadius, vSin, vCos);

	float4 newpos = float4(input.pos, 1.0f);
	newpos.y = newpos.y + dot(vSin, vWaveHeight);
	output.pos = mul(newpos, g_matWorldToProject);

	float3 binormal = float3(1.0f, 0.0f, dot(vCos, float4(1.0f, 1.0f, 1.0f, 1.0f)));
	float3 tangent = float3(0.0f, 1.0f, binormal.z);
	output.normal = cross(binormal, tangent);

	output.eyedir = g_vEyePos - input.pos;

	return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
	float4 ambinent = float4(0.0f, 0.0f, 1.0f, 1.0f);

	float3 normal = normalize(input.normal);
	float3 eyedir = normalize(input.eyedir);

	float4 diffuse = max(0.0f, dot(normal, g_vLightDir));

	float3 reflectdir = normalize(2.0f * dot(normal, g_vLightDir) * normal - g_vLightDir);
	float4 specular = pow(max(0.0f, dot(reflectdir, eyedir)), 4);

	return 0.5f*ambinent + 0.5f*diffuse + specular;
}

technique Normal
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMain();
	}
}
