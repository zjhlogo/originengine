/*!
 * \file OEFmtMesh.h
 * \date 8-8-2009 13:21:58
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEFMTMESH_H__
#define __OEFMTMESH_H__

#include "OEBasicType.h"
#include "OEMath/OEMath.h"

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
		VDM_XYZ			= 0x00000001,
		VDM_COLOR		= 0x00000002,
		VDM_UV			= 0x00000004,
		VDM_NXNYNZ		= 0x00000008,
		VDM_TXTYTZ		= 0x00000010,
		VDM_BONE		= 0x00000020,
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

#endif // __OEFMTMESH_H__
