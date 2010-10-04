/*!
 * \file OEFmtMesh.h
 * \date 8-8-2009 13:21:58
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEFMTMESH_H__
#define __OEFMTMESH_H__

#include "../libOEBase/OEBaseType.h"
#include "../libOEMath/OEMath.h"

#pragma pack(push)
#pragma pack(1)

class COEFmtMesh
{
public:
	enum CONST_DEFINE
	{
		MAGIC_NUMBER = 0x4853454D,		// MESH
		CURRENT_VERSION = 100,
		PIECE_NAME_SIZE = 32,
	};

	enum VERTEX_DATA_MASK
	{
		VDM_POSITION	= 0x00000001,
		VDM_TEXCOORD0	= 0x00000002,
		VDM_TEXCOORD1	= 0x00000004,
		VDM_TEXCOORD2	= 0x00000008,
		VDM_TEXCOORD3	= 0x00000010,
		VDM_TEXCOORD4	= 0x00000020,
		VDM_TEXCOORD5	= 0x00000040,
		VDM_TEXCOORD6	= 0x00000080,
		VDM_TEXCOORD7	= 0x00000100,
		VDM_COLOR		= 0x00000200,
		VDM_NORMAL		= 0x00000400,
		VDM_TANGENT		= 0x00000800,
		VDM_BINORMAL	= 0x00001000,
		VDM_SKELECTON	= 0x00002000,
	};

	enum PIECE_MASK
	{
		PM_VISIBLE		= 0x00000001,
		PM_TRANSPARENT	= 0x00000002,
	};

	// 1, first the file header
	typedef struct FILE_HEADER_tag
	{
		uint nMagicNumber;				// always MAGIC_NUMBER
		int nVersion;					// CURRENT_VERSION

		float fBoundingBoxMin[3];
		float fBoundingBoxMax[3];

		int nNumPieces;
	} FILE_HEADER;

	// 2, following the nNumPieces of PIECE
	typedef struct PIECE_tag
	{
		char szName[PIECE_NAME_SIZE];
		uint nPieceMask;				// see PIECE_MASK
		uint nVertexDataMask;			// see VERTEX_DATA_MASK

		int nMaterialID;				// see material id

		int nNumVerts;
		uint nOffVerts;					// sizeof(VERTEX)*nNumVerts

		int nNumIndis;
		uint nOffIndis;					// sizeof(ushort)*nNumIndis*3;
	} PIECE;
};

#pragma pack(pop)

#endif // __OEFMTMESH_H__
