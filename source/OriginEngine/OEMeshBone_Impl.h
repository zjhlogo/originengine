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
#include <OEFmtMesh.h>
#include <OEInterfaces.h>
#include <vector>

class COEMeshBone_Impl : public IOEMeshBone
{
public:
	typedef std::vector<BONE_FRAME> VBONE_FRAME;
	typedef std::vector<COEMeshBone_Impl*> VMESH_BONE;

public:
	COEMeshBone_Impl(const COEFmtMesh::BONE& Bone, int nID, IOEFile* pFile);
	virtual ~COEMeshBone_Impl();

	virtual const tstring& GetName() const;
	virtual int GetID() const;
	virtual IOEMeshBone* GetParent() const;

	virtual float GetTimeLength() const;
	virtual const CMatrix4x4& GetLocalMatrix() const;

	virtual int GetFrameCount() const;
	virtual const BONE_FRAME* GetFrame(int nIndex) const;

	virtual int GetNumChildren() const;
	virtual IOEMeshBone* GetChild(int nIndex) const;

	void SetParent(COEMeshBone_Impl* pParent);

private:
	void Init();
	void Destroy();

	void AddChild(COEMeshBone_Impl* pChild);
	void RemoveChild(COEMeshBone_Impl* pChild);

	bool Create(const COEFmtMesh::BONE& Bone, int nID, IOEFile* pFile);

private:
	tstring m_strName;
	int m_nID;

	int m_nParentID;
	COEMeshBone_Impl* m_pParent;
	VMESH_BONE m_vChildren;

	float m_fTimeLength;
	CMatrix4x4 m_matLocal;

	VBONE_FRAME m_vFrame;

};
#endif // __OEMESHBONE_IMPL_H__
