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

D3DDECLTYPE COED3DUtil::ToD3DVertType(VERT_DECL_TYPE eType)
{
	static const D3DDECLTYPE DECLARE_MAP[VDT_MAX] =
	{
		D3DDECLTYPE_UNUSED,
		D3DDECLTYPE_FLOAT1,		// 1D float expanded to (value, 0., 0., 1.)
		D3DDECLTYPE_FLOAT2,		// 2D float expanded to (value, value, 0., 1.)
		D3DDECLTYPE_FLOAT3,		// 3D float expanded to (value, value, value, 1.)
		D3DDECLTYPE_FLOAT4,		// 4D float
		D3DDECLTYPE_D3DCOLOR,	// 4D packed unsigned bytes mapped to 0. to 1. range Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
		D3DDECLTYPE_UBYTE4,		// 4D unsigned byte
	};

	assert(eType >= 0 && eType < VDT_MAX);
	return DECLARE_MAP[eType];
}

VERT_DECL_TYPE COED3DUtil::ToOEVertType(D3DDECLTYPE eType)
{
	static const int MAP_SIZE = 18;
	static const VERT_DECL_TYPE DECLARE_MAP[MAP_SIZE] =
	{
		VDT_FLOAT1,
		VDT_FLOAT2,
		VDT_FLOAT3,
		VDT_FLOAT4,
		VDT_COLOR,
		VDT_UBYTE4,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
		VDT_UNKNOWN,
	};

	assert(eType >= 0 && eType < MAP_SIZE);
	return DECLARE_MAP[eType];
}

D3DDECLUSAGE COED3DUtil::ToD3DVertUsage(VERT_DECL_USAGE eUsage)
{
	static const D3DDECLUSAGE USAGE_MAP[VDU_MAX] =
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

	assert(eUsage >= 0 && eUsage < VDU_MAX);
	return USAGE_MAP[eUsage];
}

VERT_DECL_USAGE COED3DUtil::ToOEVertUsage(D3DDECLUSAGE eUsage)
{
	static const int MAP_SIZE = 14;
	static const VERT_DECL_USAGE USAGE_MAP[MAP_SIZE] =
	{
		VDU_POSITION,
		VDU_BLENDWEIGHT,
		VDU_BLENDINDICES,
		VDU_NORMAL,
		VDU_UNKNOWN,
		VDU_TEXCOORD,
		VDU_UNKNOWN,
		VDU_UNKNOWN,
		VDU_UNKNOWN,
		VDU_POSITIONT,
		VDU_COLOR,
		VDU_UNKNOWN,
		VDU_UNKNOWN,
		VDU_UNKNOWN,
	};

	assert(eUsage >= 0 && eUsage < MAP_SIZE);
	return USAGE_MAP[eUsage];
}

int COED3DUtil::GetVertTypeSize(VERT_DECL_TYPE eType)
{
	static const int DECLARE_MAP[VDT_MAX] =
	{
		0,
		sizeof(float)*1,	// 1D float expanded to (value, 0., 0., 1.)
		sizeof(float)*2,	// 2D float expanded to (value, value, 0., 1.)
		sizeof(float)*3,	// 3D float expanded to (value, value, value, 1.)
		sizeof(float)*4,	// 4D float
		sizeof(uint),		// color
		sizeof(uchar)*4,	// ubyte4
	};

	assert(eType >= 0 && eType < VDT_MAX);
	return DECLARE_MAP[eType];
}

D3DTRANSFORMSTATETYPE COED3DUtil::ToD3DTransformType(TRANSFORM_TYPE eType)
{
	switch (eType)
	{
	case TT_WORLD:
		return D3DTS_WORLD;
		break;
	case TT_VIEW:
		return D3DTS_VIEW;
		break;
	case TT_PROJECTION:
		return D3DTS_PROJECTION;
		break;
	}

	return D3DTS_FORCE_DWORD;
}

TRANSFORM_TYPE COED3DUtil::ToOETransformType(D3DTRANSFORMSTATETYPE eType)
{
	switch (eType)
	{
	case D3DTS_WORLD:
		return TT_WORLD;
		break;
	case D3DTS_VIEW:
		return TT_VIEW;
		break;
	case D3DTS_PROJECTION:
		return TT_PROJECTION;
		break;
	}

	return TT_UNKNOWN;
}

D3DCULL COED3DUtil::ToD3DCullMode(CULL_MODE_TYPE eType)
{
	static const D3DCULL DECLARE_MAP[CMT_MAX] =
	{
		D3DCULL_NONE,
		D3DCULL_NONE,
		D3DCULL_CW,
		D3DCULL_CCW,
	};

	assert(eType >= 0 && eType < CMT_MAX);
	return DECLARE_MAP[eType];
}

CULL_MODE_TYPE COED3DUtil::ToOECullMode(D3DCULL eType)
{
	static const int MAP_SIZE = 4;
	static const CULL_MODE_TYPE DECLARE_MAP[MAP_SIZE] =
	{
		CMT_NONE,
		CMT_NONE,
		CMT_CW,
		CMT_CCW,
	};

	assert(eType >= 0 && eType < MAP_SIZE);
	return DECLARE_MAP[eType];
}

D3DFILLMODE COED3DUtil::ToD3DFillMode(FILL_MODE eType)
{
	static const D3DFILLMODE DECLARE_MAP[FM_MAX] =
	{
		D3DFILL_SOLID,
		D3DFILL_POINT,
		D3DFILL_WIREFRAME,
		D3DFILL_SOLID,
	};

	assert(eType >= 0 && eType < FM_MAX);
	return DECLARE_MAP[eType];
}

FILL_MODE COED3DUtil::ToOEFillMode(D3DFILLMODE eType)
{
	static const int MAP_SIZE = 4;
	static const FILL_MODE DECLARE_MAP[MAP_SIZE] =
	{
		FM_UNKNOWN,
		FM_POINT,
		FM_WIREFRAME,
		FM_SOLID,
	};

	assert(eType >= 0 && eType < MAP_SIZE);
	return DECLARE_MAP[eType];
}

//D3DTEXTUREFILTERTYPE COED3DUtil::ToD3DSampleFilter(OE_SAMPLE_FILTER eType)
//{
//	static const D3DTEXTUREFILTERTYPE DECLARE_MAP[OESF_MAX] =
//	{
//		D3DTEXF_NONE,
//		D3DTEXF_POINT,
//		D3DTEXF_LINEAR,
//		D3DTEXF_ANISOTROPIC,
//		D3DTEXF_PYRAMIDALQUAD,
//		D3DTEXF_GAUSSIANQUAD,
//	};
//
//	assert(eType >= 0 && eType < OESF_MAX);
//	return DECLARE_MAP[eType];
//}
//
//OE_SAMPLE_FILTER COED3DUtil::ToOESampleFilter(D3DTEXTUREFILTERTYPE eType)
//{
//	static const int MAP_SIZE = 8;
//	static const OE_SAMPLE_FILTER DECLARE_MAP[MAP_SIZE] =
//	{
//		OESF_UNKNOWN,
//		OESF_POINT,
//		OESF_LINEAR,
//		OESF_ANISOTROPIC,
//		OESF_UNKNOWN,
//		OESF_UNKNOWN,
//		OESF_PYRAMIDALQUAD,
//		OESF_GAUSSIANQUAD,
//	};
//
//	assert(eType >= 0 && eType < MAP_SIZE);
//	return DECLARE_MAP[eType];
//}

D3DFORMAT COED3DUtil::ToD3DTexFmt(TEXTURE_FORMAT eFormat)
{
	switch (eFormat)
	{
	case TF_A8R8G8B8:
		return D3DFMT_A8R8G8B8;
		break;
	case TF_A4R4G4B4:
		return D3DFMT_A4R4G4B4;
		break;
	}

	return D3DFMT_UNKNOWN;
}

TEXTURE_FORMAT COED3DUtil::ToOETexFmt(D3DFORMAT eFormat)
{
	switch (eFormat)
	{
	case D3DFMT_A8R8G8B8:
		return TF_A8R8G8B8;
		break;
	case D3DFMT_A4R4G4B4:
		return TF_A4R4G4B4;
		break;
	}

	return TF_UNKNOWN;
}

uint COED3DUtil::ToD3DColorWriteChannel(uint nColorWriteChannel)
{
	// so far they are the same value
	return nColorWriteChannel;
}

uint COED3DUtil::ToOEColorWriteChannel(uint nColorWriteChannel)
{
	// so far they are the same value
	return nColorWriteChannel;
}
