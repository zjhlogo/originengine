/*!
 * \file OEFmtSkeleton.h
 * \date 1-3-2010 20:49:48
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEFMTSKELETON_H__
#define __OEFMTSKELETON_H__

#include "../libOEBase/OEBaseType.h"
#include "../libOEMath/OEMath.h"

#pragma pack(push)
#pragma pack(1)

class COEFmtSkeleton
{
public:
	enum CONST_DEFINE
	{
		MAGIC_NUMBER = 0x454E4F42,		// BONE
		CURRENT_VERSION = 100,
		BONE_NAME_SIZE = 32,
		INVALID_BONE_ID = -1,
	};

	enum SKELETON_MASK
	{
		SM_UNKNOWN			= 0x00000000,
		SM_LOOP_ANIM		= 0x00000001,
		SM_CLAMP_ANIM		= 0x00000002,
	};

	// 1, first the file header
	typedef struct FILE_HEADER_tag
	{
		uint nMagicNumber;				// always MAGIC_NUMBER
		int nVersion;					// CURRENT_VERSION

		int nNumBones;
	} FILE_HEADER;

	// 2, following the nNumBones of BONE
	typedef struct BONE_tag
	{
		char szName[BONE_NAME_SIZE];
		int nParentIndex;

		float fTimeLength;
		float matLocal[16];

		uint nBoneMask;					// see BONE_MASK

		int nNumFrameRot;
		uint nOffFrameRot;

		int nNumFramePos;
		uint nOffFramePos;

		int nNumFrameScale;
		uint nOffFrameScale;
	} BONE;

	// 3, bone rotation / translation / scale data
	typedef struct FRAME_ROT_tag
	{
		float fTime;
		float qRot[4];
	} FRAME_ROT;

	typedef struct FRAME_POS_tag
	{
		float fTime;
		float vPos[3];
	} FRAME_POS;

	typedef struct FRAME_SCALE_tag
	{
		float fTime;
		float vScale[3];
	} FRAME_SCALE;
};

#pragma pack(pop)

#endif // __OEFMTSKELETON_H__
