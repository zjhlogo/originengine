/*!
 * \file IOEMesh.h
 * \date 30-7-2009 10:04:53
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMESH_H__
#define __IOEMESH_H__

#include "../libOEBase/IOEObject.h"
#include "IOEPiece.h"
#include "IOEBone.h"

class IOEMesh : public IOEObject
{
public:
	IOEMesh() {};
	virtual ~IOEMesh() {};

	virtual int GetNumPieces() const = 0;
	virtual IOEPiece* GetPiece(int nIndex) const = 0;
	virtual IOEPiece* FindPiece(const tstring& strName) const = 0;

	virtual const CVector3& GetBoundingBoxMin() const = 0;
	virtual const CVector3& GetBoundingBoxMax() const = 0;

};
#endif // __IOEMESH_H__
