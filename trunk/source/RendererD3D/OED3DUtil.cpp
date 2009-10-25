/*!
 * \file OED3DUtil.cpp
 * \date 24-5-2009 18:03:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DUtil.h"
#include <assert.h>

void COED3DUtil::ToD3DXMatrix(D3DXMATRIX& matOut, const CMatrix4x4& matIn)
{
	memcpy(matOut.m, matIn.m, sizeof(matIn.m));
}

void COED3DUtil::ToOEMatrix(CMatrix4x4& matOut, const D3DXMATRIX& matIn)
{
	memcpy(matOut.m, matIn.m, sizeof(matIn.m));
}

void COED3DUtil::ToD3DVector3(D3DXVECTOR3& vOut, const CVector3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.y;
	vOut.z = vIn.z;
}

void COED3DUtil::ToOEVector3(CVector3& vOut, const D3DXVECTOR3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.y;
	vOut.z = vIn.z;
}

void COED3DUtil::ToD3DVector4(D3DXVECTOR4& vOut, const CVector4& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.y;
	vOut.z = vIn.z;
	vOut.w = vIn.w;
}

void COED3DUtil::ToOEVector4(CVector4& vOut, const D3DXVECTOR4& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.y;
	vOut.z = vIn.z;
	vOut.w = vIn.w;
}

D3DDECLTYPE COED3DUtil::ToD3DVertType(IOEVertDecl::TYPE eType)
{
	static const D3DDECLTYPE DECLARE_MAP[IOEVertDecl::T_MAX] =
	{
		D3DDECLTYPE_UNUSED,
		D3DDECLTYPE_FLOAT1,		// 1D float expanded to (value, 0., 0., 1.)
		D3DDECLTYPE_FLOAT2,		// 2D float expanded to (value, value, 0., 1.)
		D3DDECLTYPE_FLOAT3,		// 3D float expanded to (value, value, value, 1.)
		D3DDECLTYPE_FLOAT4,		// 4D float
		D3DDECLTYPE_D3DCOLOR,	// 4D packed unsigned bytes mapped to 0. to 1. range Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
		D3DDECLTYPE_UBYTE4,		// 4D unsigned byte
	};

	assert(eType >= 0 && eType < IOEVertDecl::T_MAX);
	return DECLARE_MAP[eType];
}

IOEVertDecl::TYPE COED3DUtil::ToOEVertType(D3DDECLTYPE eType)
{
	static const int MAP_SIZE = 18;
	static const IOEVertDecl::TYPE DECLARE_MAP[MAP_SIZE] =
	{
		IOEVertDecl::T_FLOAT1,
		IOEVertDecl::T_FLOAT2,
		IOEVertDecl::T_FLOAT3,
		IOEVertDecl::T_FLOAT4,
		IOEVertDecl::T_COLOR,
		IOEVertDecl::T_UBYTE4,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
	};

	assert(eType >= 0 && eType < MAP_SIZE);
	return DECLARE_MAP[eType];
}

D3DDECLUSAGE COED3DUtil::ToD3DVertUsage(IOEVertDecl::USAGE eUsage)
{
	static const D3DDECLUSAGE USAGE_MAP[IOEVertDecl::U_MAX] =
	{
		D3DDECLUSAGE_POSITION,		// 0
		D3DDECLUSAGE_POSITION,		// 0
		D3DDECLUSAGE_POSITIONT,		// 9
		D3DDECLUSAGE_NORMAL,        // 3
		D3DDECLUSAGE_TEXCOORD,      // 5
		D3DDECLUSAGE_COLOR,         // 10
		D3DDECLUSAGE_BLENDWEIGHT,	// 1
		D3DDECLUSAGE_BLENDINDICES,	// 2
	};

	assert(eUsage >= 0 && eUsage < IOEVertDecl::U_MAX);
	return USAGE_MAP[eUsage];
}

IOEVertDecl::USAGE COED3DUtil::ToOEVertUsage(D3DDECLUSAGE eUsage)
{
	static const int MAP_SIZE = 14;
	static const IOEVertDecl::USAGE USAGE_MAP[MAP_SIZE] =
	{
		IOEVertDecl::U_POSITION,
		IOEVertDecl::U_BLENDWEIGHT,
		IOEVertDecl::U_BLENDINDICES,
		IOEVertDecl::U_NORMAL,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_TEXCOORD,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_POSITIONT,
		IOEVertDecl::U_COLOR,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
	};

	assert(eUsage >= 0 && eUsage < MAP_SIZE);
	return USAGE_MAP[eUsage];
}

int COED3DUtil::GetVertTypeSize(IOEVertDecl::TYPE eType)
{
	static const int DECLARE_MAP[IOEVertDecl::T_MAX] =
	{
		0,
		sizeof(float)*1,	// 1D float expanded to (value, 0., 0., 1.)
		sizeof(float)*2,	// 2D float expanded to (value, value, 0., 1.)
		sizeof(float)*3,	// 3D float expanded to (value, value, value, 1.)
		sizeof(float)*4,	// 4D float
		sizeof(uint),		// color
		sizeof(uchar)*4,	// ubyte4
	};

	assert(eType >= 0 && eType < IOEVertDecl::T_MAX);
	return DECLARE_MAP[eType];
}

D3DTRANSFORMSTATETYPE COED3DUtil::ToD3DTransformType(IOERenderer::TRANSFORM_TYPE eType)
{
	switch (eType)
	{
	case IOERenderer::TT_WORLD:
		return D3DTS_WORLD;
		break;
	case IOERenderer::TT_VIEW:
		return D3DTS_VIEW;
		break;
	case IOERenderer::TT_PROJECTION:
		return D3DTS_PROJECTION;
		break;
	}

	return D3DTS_FORCE_DWORD;
}

IOERenderer::TRANSFORM_TYPE COED3DUtil::ToOETransformType(D3DTRANSFORMSTATETYPE eType)
{
	switch (eType)
	{
	case D3DTS_WORLD:
		return IOERenderer::TT_WORLD;
		break;
	case D3DTS_VIEW:
		return IOERenderer::TT_VIEW;
		break;
	case D3DTS_PROJECTION:
		return IOERenderer::TT_PROJECTION;
		break;
	}

	return IOERenderer::TT_UNKNOWN;
}

D3DCULL COED3DUtil::ToD3DCullMode(IOERenderer::CULL_MODE_TYPE eType)
{
	static const D3DCULL DECLARE_MAP[IOERenderer::CMT_MAX] =
	{
		D3DCULL_NONE,
		D3DCULL_NONE,
		D3DCULL_CW,
		D3DCULL_CCW,
	};

	assert(eType >= 0 && eType < IOERenderer::CMT_MAX);
	return DECLARE_MAP[eType];
}

IOERenderer::CULL_MODE_TYPE COED3DUtil::ToOECullMode(D3DCULL eType)
{
	static const int MAP_SIZE = 4;
	static const IOERenderer::CULL_MODE_TYPE DECLARE_MAP[MAP_SIZE] =
	{
		IOERenderer::CMT_NONE,
		IOERenderer::CMT_NONE,
		IOERenderer::CMT_CW,
		IOERenderer::CMT_CCW,
	};

	assert(eType >= 0 && eType < MAP_SIZE);
	return DECLARE_MAP[eType];
}

D3DFILLMODE COED3DUtil::ToD3DFillMode(IOERenderer::FILL_MODE eType)
{
	static const D3DFILLMODE DECLARE_MAP[IOERenderer::FM_MAX] =
	{
		D3DFILL_SOLID,
		D3DFILL_POINT,
		D3DFILL_WIREFRAME,
		D3DFILL_SOLID,
	};

	assert(eType >= 0 && eType < IOERenderer::FM_MAX);
	return DECLARE_MAP[eType];
}

IOERenderer::FILL_MODE COED3DUtil::ToOEFillMode(D3DFILLMODE eType)
{
	static const int MAP_SIZE = 4;
	static const IOERenderer::FILL_MODE DECLARE_MAP[MAP_SIZE] =
	{
		IOERenderer::FM_UNKNOWN,
		IOERenderer::FM_POINT,
		IOERenderer::FM_WIREFRAME,
		IOERenderer::FM_SOLID,
	};

	assert(eType >= 0 && eType < MAP_SIZE);
	return DECLARE_MAP[eType];
}

D3DTEXTUREFILTERTYPE COED3DUtil::ToD3DSampleFilter(IOERenderer::SAMPLE_FILTER eType)
{
	static const D3DTEXTUREFILTERTYPE DECLARE_MAP[IOERenderer::SF_MAX] =
	{
		D3DTEXF_NONE,
		D3DTEXF_POINT,
		D3DTEXF_LINEAR,
		D3DTEXF_ANISOTROPIC,
		D3DTEXF_PYRAMIDALQUAD,
		D3DTEXF_GAUSSIANQUAD,
	};

	assert(eType >= 0 && eType < IOERenderer::SF_MAX);
	return DECLARE_MAP[eType];
}

IOERenderer::SAMPLE_FILTER COED3DUtil::ToOESampleFilter(D3DTEXTUREFILTERTYPE eType)
{
	static const int MAP_SIZE = 8;
	static const IOERenderer::SAMPLE_FILTER DECLARE_MAP[MAP_SIZE] =
	{
		IOERenderer::SF_UNKNOWN,
		IOERenderer::SF_POINT,
		IOERenderer::SF_LINEAR,
		IOERenderer::SF_ANISOTROPIC,
		IOERenderer::SF_UNKNOWN,
		IOERenderer::SF_UNKNOWN,
		IOERenderer::SF_PYRAMIDALQUAD,
		IOERenderer::SF_GAUSSIANQUAD,
	};

	assert(eType >= 0 && eType < MAP_SIZE);
	return DECLARE_MAP[eType];
}

D3DFORMAT COED3DUtil::ToD3DTexFmt(IOETexture::TEXTURE_FORMAT eFormat)
{
	switch (eFormat)
	{
	case IOETexture::TF_A8R8G8B8:
		return D3DFMT_A8R8G8B8;
		break;
	case IOETexture::TF_A4R4G4B4:
		return D3DFMT_A4R4G4B4;
		break;
	}

	return D3DFMT_UNKNOWN;
}

IOETexture::TEXTURE_FORMAT COED3DUtil::ToOETexFmt(D3DFORMAT eFormat)
{
	switch (eFormat)
	{
	case D3DFMT_A8R8G8B8:
		return IOETexture::TF_A8R8G8B8;
		break;
	case D3DFMT_A4R4G4B4:
		return IOETexture::TF_A4R4G4B4;
		break;
	}

	return IOETexture::TF_UNKNOWN;
}
