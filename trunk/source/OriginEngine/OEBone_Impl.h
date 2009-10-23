/*!
 * \file OEBone_Impl.h
 * \date 10-23-2009 13:56:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBONE_IMPL_H__
#define __OEBONE_IMPL_H__

#include <IOEBone.h>
#include <IOEFile.h>
#include <OEFmtMesh.h>

#include <vector>

class COEBone_Impl : public IOEBone
{
public:
	typedef std::vector<BONE_FRAME> VBONE_FRAME;

public:
	COEBone_Impl(const COEFmtMesh::BONE& Bone, int nID, IOEFile* pFile);
	virtual ~COEBone_Impl();

	virtual const tstring& GetName() const;
	virtual int GetID() const;
	virtual int GetParentID() const;

	virtual float GetTimeLength() const;
	virtual const CMatrix4x4& GetLocalMatrix() const;
	virtual const CMatrix4x4& GetWorldMatrix() const;
	virtual const CMatrix4x4& GetWorldMatrixInv() const;

	virtual int GetFrameCount() const;
	virtual const BONE_FRAME* GetFrame(int nIndex) const;

	virtual bool SlerpMatrix(CMatrix4x4& matOut, float fTime, bool bLoop = true);
	void SetWorldMatrix(const CMatrix4x4& matWorld);

private:
	void Init();
	void Destroy();

	bool Create(const COEFmtMesh::BONE& Bone, int nID, IOEFile* pFile);

private:
	tstring m_strName;
	int m_nID;
	int m_nParentID;

	float m_fTimeLength;

	CMatrix4x4 m_matLocal;
	CMatrix4x4 m_matWorld;
	CMatrix4x4 m_matWorldInv;

	VBONE_FRAME m_vFrame;

};
#endif // __OEBONE_IMPL_H__
