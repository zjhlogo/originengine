/*!
 * \file OEBaseTypeEx.h
 * \date 13-2-2010 10:33:52
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBASETYPEEX_H__
#define __OEBASETYPEEX_H__

#include "OEBaseType.h"
#include <vector>

enum TRANSFORM_TYPE
{
	TT_UNKNOWN			= 0x00000000,
	TT_WORLD			= 0x00000001,
	TT_VIEW				= 0x00000002,
	TT_PROJECTION		= 0x00000004,

	TT_WORLD_VIEW		= TT_WORLD|TT_VIEW,					// just for GetTransform
	TT_VIEW_PROJ		= TT_VIEW|TT_PROJECTION,			// just for GetTransform
	TT_WORLD_VIEW_PROJ	= TT_WORLD|TT_VIEW|TT_PROJECTION,	// just for GetTransform
};

enum CULL_MODE_TYPE
{
	CMT_UNKNOWN = 0,
	CMT_NONE,
	CMT_CW,
	CMT_CCW,
	CMT_MAX,
};

enum FILL_MODE
{
	FM_UNKNOWN = 0,
	FM_POINT,
	FM_WIREFRAME,
	FM_SOLID,
	FM_MAX,
};

//enum OE_SAMPLE_FILTER
//{
//	OESF_UNKNOWN = 0,
//	OESF_POINT,
//	OESF_LINEAR,
//	OESF_ANISOTROPIC,
//	OESF_PYRAMIDALQUAD,
//	OESF_GAUSSIANQUAD,
//	OESF_MAX,
//};

enum TEXTURE_FORMAT
{
	TF_UNKNOWN,
	TF_A8R8G8B8,
	TF_A4R4G4B4,
	TF_MAX,
};

enum VERT_DECL_TYPE
{
	VDT_UNKNOWN = 0,
	VDT_FLOAT1,
	VDT_FLOAT2,
	VDT_FLOAT3,
	VDT_FLOAT4,
	VDT_COLOR,
	VDT_UBYTE4,
	VDT_MAX,
};

enum VERT_DECL_USAGE
{
	VDU_UNKNOWN = 0,
	VDU_POSITION,
	VDU_POSITIONT,
	VDU_NORMAL,
	VDU_TEXCOORD,
	VDU_COLOR,
	VDU_BLENDWEIGHT,
	VDU_BLENDINDICES,
	VDU_MAX,
};

typedef struct VERT_DECL_ELEMENT_tag
{
	VERT_DECL_TYPE eType;
	VERT_DECL_USAGE eUsage;
	uint nIndex;
} VERT_DECL_ELEMENT;

enum ELEMENT_CONST_DEFINE
{
	MAX_ELEMENT_COUNT = 32,
};

enum OE_RENDER_DATA_TYPE
{
	OERDT_UNKNOWN = 0,
	OERDT_SKINMESH,

	OERDT_USER = 1000,
};

enum OE_CONTROL_TYPE
{
	OECT_UNKNOWN = 0,
	OECT_SKINMESH,

	OECT_USER = 1000,
};

enum OE_RENDER_TYPE
{
	OERT_UNKNOWN = 0,
	OERT_SKINMESH,
	OERT_BOUNDINGBOX,
	OERT_SKELETON,

	OERT_USER = 1000,
};

enum DEFAULT_SHADER_TYPE
{
	DST_UNKNOWN = 0,
	DST_LINE,
	DST_POLYC,
	DST_POLYT,
	DST_POLY_UI,
	DST_NUM,
};

typedef struct VERTEX_LINE_tag
{
	float x, y, z;
	uint nColor;
} VERTEX_LINE;

typedef std::vector<VERTEX_LINE> TV_VERTEX_LINE;

typedef struct VERTEX_POLYC_tag
{
	float x, y, z;
	uint nColor;
} VERTEX_POLYC;

typedef std::vector<VERTEX_POLYC> TV_VERTEX_POLYC;

typedef struct VERTEX_POLYT_tag
{
	float x, y, z;
	float u, v;
} VERTEX_POLYT;

typedef std::vector<VERTEX_POLYT> TV_VERTEX_POLYT;

typedef struct VERTEX_POLY_UI_tag
{
	float x, y, z, w;
	uint nColor;
	float u, v;
} VERTEX_POLY_UI;

typedef std::vector<VERTEX_POLY_UI> TV_VERTEX_POLY_UI;

typedef std::vector<ushort> TV_VERTEX_INDEX;

enum MATERIAL_TEXTURE_TYPE
{
	MTT_DIFFUSE = 0,
	MTT_NORMAL,
	MTT_TEXTURE2,
	MTT_TEXTURE3,
	MTT_TEXTURE4,
	MTT_TEXTURE5,
	MTT_TEXTURE6,
	MTT_TEXTURE7,
	MTT_NUM,
};

#endif // __OEBASETYPEEX_H__
