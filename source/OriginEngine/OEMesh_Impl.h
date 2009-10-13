/*!
 * \file OEMesh_Impl.h
 * \date 30-7-2009 16:50:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMESH_IMPL_H__
#define __OEMESH_IMPL_H__

#include <IOEMesh.h>
#include "OEMeshPiece_Impl.h"
#include "OEMeshBone_Impl.h"

#include <vector>

class COEMesh_Impl : public IOEMesh
{
public:
	typedef std::vector<COEMeshPiece_Impl*> VMESH_PIECE;
	typedef std::vector<COEMeshBone_Impl*> VMESH_BONE;

public:
	COEMesh_Impl(const tstring& strFileName);
	virtual ~COEMesh_Impl();

	virtual int GetNumPieces() const;
	virtual IOEMeshPiece* GetPiece(int nIndex) const;
	virtual IOEMeshPiece* FindPiece(const tstring& strName) const;

	virtual int GetNumBones() const;
	virtual IOEMeshBone* GetBone(int nIndex) const;
	virtual IOEMeshBone* FindBone(const tstring& strName) const;

	virtual IOEMeshBone* GetRootBone() const;

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFileName);

private:
	VMESH_PIECE m_vPiece;
	VMESH_BONE m_vBone;

};

#endif // __OEMESH_IMPL_H__
