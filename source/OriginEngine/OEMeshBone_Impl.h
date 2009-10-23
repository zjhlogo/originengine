/*!
 * \file OEMeshBone_Impl.h
 * \date 8-8-2009 22:03:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMESHBONE_IMPL_H__
#define __OEMESHBONE_IMPL_H__

#include <IOEMeshBone.h>

#include <IOEFile.h>
#include <OEFmtMesh.h>

#include <vector>

class COEMeshBone_Impl : public IOEMeshBone
{
public:
	typedef std::vector<BONE_FRAME> VBONE_FRAME;

public:
	COEMeshBone_Impl(const COEFmtMesh::BONE& Bone, int nID, IOEFile* pFile);
	virtual ~COEMeshBone_Impl();

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

#endif // __OEMESHBONE_IMPL_H__
