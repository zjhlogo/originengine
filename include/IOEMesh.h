/*!
 * \file IOEMesh.h
 * \date 30-7-2009 10:04:53
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMESH_H__
#define __IOEMESH_H__

#include "IOEObject.h"
#include "IOEMeshPiece.h"
#include "IOEMeshBone.h"

class IOEMesh : public IOEObject
{
public:
	IOEMesh() {};
	virtual ~IOEMesh() {};

	virtual int GetNumPieces() const = 0;
	virtual IOEMeshPiece* GetPiece(int nIndex) const = 0;
	virtual IOEMeshPiece* FindPiece(const tstring& strName) const = 0;

	virtual int GetNumBones() const = 0;
	virtual IOEMeshBone* GetBone(int nIndex) const = 0;
	virtual IOEMeshBone* FindBone(const tstring& strName) const = 0;

	virtual IOEMeshBone* GetRootBone() const = 0;

};
#endif // __IOEMESH_H__
