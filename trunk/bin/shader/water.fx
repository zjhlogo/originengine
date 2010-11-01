float4x4 g_matWorldToProject;
texture g_texReflect;
texture g_texRefract;
texture g_texNormal;
float3 g_vEyePos;
float g_fTime;

sampler sampleReflect =
sampler_state
{
	Texture = <g_texReflect>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
    AddressV = CLAMP;
};

sampler sampleRefract =
sampler_state
{
	Texture = <g_texRefract>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
    AddressV = CLAMP;
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
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float4 texcoord : TEXCOORD0;
	float3 eyedir : TEXCOORD1;
    float4 wave0 : TEXCOORD2;
    float2 wave1 : TEXCOORD3;
    float2 wave2 : TEXCOORD4;
	float2 wave3 : TEXCOORD5;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

    output.pos = mul(float4(input.pos, 1.0f), g_matWorldToProject);

	// perspective corrected projection
    output.texcoord.xy = float2(output.pos.w+output.pos.x, output.pos.w-output.pos.y)*0.5f;
    output.texcoord.zw = float2(1.0f, output.pos.w);

	float3 binormal = cross(input.tangent, input.normal);
	float3x3 TangentToModel = float3x3(input.tangent, binormal, input.normal);

	float3 eyeDir = g_vEyePos - input.pos;
	output.eyedir = mul(eyeDir, TangentToModel);

	float2 fTranslation = float2(g_fTime*0.005f, 0.0f);
	float2 vTexCoords = float2(input.pos.x, -input.pos.z)*0.001f;
	float fSinTranslation = sin(fTranslation*100.0f)*0.005f;
    float2 vTranslation0=fTranslation+fSinTranslation;
    float2 vTranslation1=fTranslation-fSinTranslation;
    float2 vTranslation2=fTranslation;

	output.wave0.xy = vTexCoords.xy+fTranslation*2.0f;
	output.wave0.zw = output.pos.w;
    output.wave1.xy = vTexCoords.xy*2.0f+fTranslation*4.0f;
    output.wave2.xy = vTexCoords.xy*4.0f+fTranslation*2.0f;
    output.wave3.xy = vTexCoords.xy*8.0f+fTranslation;

	return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR
{
	float3 eyeDir = normalize(input.eyedir);

	float3 vBumpTexA = tex2D(sampleNormal, input.wave0.xy).xyz;
	float3 vBumpTexB = tex2D(sampleNormal, input.wave1.xy).xyz;
	float3 vBumpTexC = tex2D(sampleNormal, input.wave2.xy).xyz;
	float3 vBumpTexD = tex2D(sampleNormal, input.wave3.xy).xyz;

	float3 normal = normalize(2.0f*(vBumpTexA.xyz+vBumpTexB.xyz+vBumpTexC.xyz+vBumpTexD.xyz)-4.0f);

	float3 reflectNormal = normal * float3(0.1f, 0.1f, 1.0f);
	float3 refractNormal = normal * float3(0.02f, 0.02f, 1.0f);

	float2 texcoordProj = input.texcoord.xy/input.texcoord.w;
	float4 colorReflect = tex2D(sampleReflect, texcoordProj + reflectNormal.xy);
	float4 colorRefractA = tex2D(sampleRefract, texcoordProj + refractNormal.xy);
	float4 colorRefractB = tex2D(sampleRefract, texcoordProj);

	float4 colorRefract = colorRefractB*colorRefractA.w + colorRefractA*(1.0f-colorRefractA.w);

	float NdotL = max(dot(eyeDir, reflectNormal), 0.0f);
	float facing = (1.0f - NdotL);
	float fresnel = max(0.2f + (1.0f-0.2f)*pow(facing, 5.0f), 0.0f);

	float fDistScale = saturate(10.0f/input.wave0.w);
	float3 waterDeepColor = (colorRefract.xyz*fDistScale + (1.0f-fDistScale)*float3(0.0f, 0.1f, 0.125f));

	float3 waterColor = float3(0.0f, 0.1f, 0.15f)*facing + waterDeepColor*(1.0f-facing);

	return float4(colorReflect*fresnel + waterColor, 1.0f);
}

technique Normal
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMain();
	}
}
