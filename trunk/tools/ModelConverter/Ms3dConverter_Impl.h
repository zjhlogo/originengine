/*!
 * \file Ms3dConverter_Impl.h
 * \date 1-3-2010 21:19:41
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MS3DCONVERTER_IMPL_H__
#define __MS3DCONVERTER_IMPL_H__

#include "IConverter.h"
#include "FmtMs3d.h"

#include <OEFmtMesh.h>
#include <OEFmtSkeleton.h>

#include <vector>

class CMs3dConverter_Impl : public IConverter
{
public:
	typedef struct VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		int nBoneID;
	} VERTEX;

	typedef std::vector<VERTEX> TV_VERTEX;

	typedef struct INDEX_tag
	{
		ushort v1, v2, v3;
	} INDEX;
	typedef std::vector<INDEX> TV_INDEX;

	typedef struct FILE_VERTEX_tag
	{
		float x, y, z;
		float u, v;
		uchar nBoneIndex[4];
		float fWeight[4];
	} FILE_VERTEX;

	typedef std::vector<COEFmtSkeleton::FRAME_ROT> TV_FRAME_ROT;
	typedef std::vector<COEFmtSkeleton::FRAME_POS> TV_FRAME_POS;
	typedef std::vector<COEFmtSkeleton::FRAME_SCALE> TV_FRAME_SCALE;

	typedef struct BONE_INFO_tag
	{
		tstring strName;
		tstring strParentName;
		int nIndex;
		int nParentIndex;

		float fTimeLength;
		CMatrix4x4 matLocal;

		TV_FRAME_ROT vFrameRot;
		TV_FRAME_POS vFramePos;
		TV_FRAME_SCALE vFrameScale;
	} BONE_INFO;
	typedef std::vector<BONE_INFO> TV_BONE_INFO;

public:
	CMs3dConverter_Impl();
	virtual ~CMs3dConverter_Impl();

	virtual bool CanConvert(const tstring& strFile);
	virtual bool DoConvert(const tstring& strFileIn, const tstring& strFileOut);

private:
	void Init();
	void Destroy();

	bool LoadFromFile(const tstring& strFile);
	bool SaveToMeshFile(const tstring& strFile);
	bool SaveToBoneFile(const tstring& strFile);

	void Ms3dRot2OERot(CQuaternion& qOut, const float* rRot);
	void Ms3dPos2OEPos(CVector3& vOut, const float* vPos);
	void Ms3dScale2OEScale(CVector3& vScale);
	void Ms3dTrans2OETrans(CMatrix4x4& matOut, const float* rRot, const float* vPos);

	int FindParentIndex(const TV_BONE_INFO& vBoneInfo, const tstring& strParentName);
	void SetupParentBoneIndex(TV_BONE_INFO& vBoneInfo);

private:
	TV_VERTEX m_vVerts;
	TV_INDEX m_vIndis;
	TV_BONE_INFO m_vBoneInfo;

};
#endif // __MS3DCONVERTER_IMPL_H__
