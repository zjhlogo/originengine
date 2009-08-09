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
		MAGIC_NUMBER = 0x4853454D,
		CURRENT_VERSION = 100,
		PIECE_NAME_SIZE = 32,
		BONE_NAME_SIZE = 32,
		INVALID_BONE_ID = -1,
	};

	enum VERTEX_DATA_MASK
	{
		VDM_XYZ			= 0x00000001,
		VDM_COLOR		= 0x00000002,
		VDM_NXNYNZ		= 0x00000004,
		VDM_UV			= 0x00000008,
		VDM_BONE		= 0x00000010,
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
		int nNumBones;
	} FILE_HEADER;

	// 2, following the nNumPieces of PIECE
	typedef struct PIECE_tag
	{
		char szName[PIECE_NAME_SIZE];
		uint nPieceMask;				// see PIECE_MASK
		uint nVertexDataMask;			// see VERTEX_DATA_MASK

		int nMaterialID;				// see material.xml

		int nNumVerts;
		uint nOffVerts;					// sizeof(VERTEX)*nNumVerts

		int nNumIndis;
		uint nOffIndis;					// sizeof(ushort)*nNumIndis*3;
	} PIECE;

	// 3, following the nNumBones of BONE
	typedef struct BONE_FRAME_tag
	{
		float fTime;
		float matTransform[16];
	} BONE_FRAME;

	typedef struct BONE_tag
	{
		char szName[BONE_NAME_SIZE];
		int nParentIndex;
		float fTimeLength;
		float matLocal[16];
		int nNumBoneFrame;
		uint nOffBoneFrame;
	} BONE;

};
#endif // __OEFMTMESH_H__
