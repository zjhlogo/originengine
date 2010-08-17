/*!
 * \file OEBone_Impl.h
 * \date 10-23-2009 13:56:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBONE_IMPL_H__
#define __OEBONE_IMPL_H__

#include <OECore/IOEBone.h>
#include <OEBase/IOEFile.h>
#include <OECore/OEFmtSkeleton.h>

#include <vector>

class COEBone_Impl : public IOEBone
{
public:
	typedef struct KEYFRAME_ROT_tag
	{
		float fTime;
		CQuaternion qRot;
	} KEYFRAME_ROT;

	typedef struct KEYFRAME_POS_tag
	{
		float fTime;
		CVector3 vPos;
	} KEYFRAME_POS;

	typedef struct KEYFRAME_SCALE_tag
	{
		float fTime;
		CVector3 vScale;
	} KEYFRAME_SCALE;

	typedef std::vector<KEYFRAME_ROT> TV_KEYFRAME_ROT;
	typedef std::vector<KEYFRAME_POS> TV_KEYFRAME_POS;
	typedef std::vector<KEYFRAME_SCALE> TV_KEYFRAME_SCALE;

public:
	COEBone_Impl(const COEFmtSkeleton::BONE& Bone, int nID, IOEFile* pFile);
	virtual ~COEBone_Impl();

	virtual const tstring& GetName() const;
	virtual int GetID() const;
	virtual int GetParentID() const;

	virtual float GetTimeLength() const;
	virtual const CMatrix4x4& GetLocalMatrix() const;
	virtual const CMatrix4x4& GetWorldMatrix() const;
	virtual const CMatrix4x4& GetWorldMatrixInv() const;

	virtual bool SlerpMatrix(CMatrix4x4& matOut, float fTime, bool bLoop = true);
	void SetWorldMatrix(const CMatrix4x4& matWorld);

private:
	void Init();
	void Destroy();

	bool Create(const COEFmtSkeleton::BONE& Bone, int nID, IOEFile* pFile);

	bool SlerpRot(CQuaternion& qRotOut, float fTime);
	bool LerpPos(CVector3& vPosOut, float fTime);
	bool LerpScale(CVector3& vScaleOut, float fTime);

private:
	tstring m_strName;
	int m_nID;
	int m_nParentID;

	float m_fTimeLength;

	CMatrix4x4 m_matLocal;
	CMatrix4x4 m_matWorld;
	CMatrix4x4 m_matWorldInv;

	CQuaternion m_qLocalRot;
	CVector3 m_vLocalPos;
	CVector3 m_vLocalScale;

	TV_KEYFRAME_ROT m_vKeyFrameRot;
	TV_KEYFRAME_POS m_vKeyFramePos;
	TV_KEYFRAME_SCALE m_vKeyFrameScale;

};
#endif // __OEBONE_IMPL_H__
