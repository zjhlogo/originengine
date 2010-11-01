float4x4 g_matWorldToProject;

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
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

    output.pos = mul(float4(input.pos, 1.0f), g_matWorldToProject);
	return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

technique Normal
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMain();
	}
}
