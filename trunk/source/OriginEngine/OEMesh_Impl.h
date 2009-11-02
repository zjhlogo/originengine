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
#include "OEPiece_Impl.h"
#include "OEBone_Impl.h"

#include <vector>

class COEMesh_Impl : public IOEMesh
{
public:
	typedef std::vector<COEPiece_Impl*> TV_PIECE;

public:
	COEMesh_Impl(const tstring& strFileName);
	virtual ~COEMesh_Impl();

	virtual int GetNumPieces() const;
	virtual IOEPiece* GetPiece(int nIndex) const;
	virtual IOEPiece* FindPiece(const tstring& strName) const;

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFile);

private:
	TV_PIECE m_vPiece;

};

#endif // __OEMESH_IMPL_H__
