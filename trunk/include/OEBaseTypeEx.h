/*!
 * \file OEBaseTypeEx.h
 * \date 13-2-2010 10:33:52
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBASETYPEEX_H__
#define __OEBASETYPEEX_H__

#include "OEBasicType.h"
#include "OEMath/OEMath.h"
#include <vector>

class IOEShader;
class IOETexture;
class IOEPiece;
class IOEBone;

typedef std::vector<CMatrix4x4> TV_MATRIX;
typedef std::vector<IOEPiece*> TV_PIECE;
typedef std::vector<IOEBone*> TV_BONE;

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

enum OE_SAMPLE_FILTER
{
	OESF_UNKNOWN = 0,
	OESF_POINT,
	OESF_LINEAR,
	OESF_ANISOTROPIC,
	OESF_PYRAMIDALQUAD,
	OESF_GAUSSIANQUAD,
	OESF_MAX,
};

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

typedef struct MATERIAL_tag
{
	int nID;
	int nVertDecl;
	tstring strShaderFile;
	tstring strTextureFile;
	tstring strTexNormalFile;
	IOEShader* pShader;
	IOETexture* pTexture;
	IOETexture* pTexNormal;
	// TODO: some additional parameter
} MATERIAL;

typedef std::vector<MATERIAL> TV_MATERIAL;

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
	OERT_BOUNDBOX,
	OERT_SKELECTON,

	OERT_USER = 1000,
};

#endif // __OEBASETYPEEX_H__
