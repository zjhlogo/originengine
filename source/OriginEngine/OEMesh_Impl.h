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
#include <IOEResMgr.h>

class COEMesh_Impl : public IOEMesh
{
public:
	COEMesh_Impl(const tstring& strFile);
	virtual ~COEMesh_Impl();

	virtual int GetNumPieces() const;
	virtual IOEPiece* GetPiece(int nIndex) const;
	virtual IOEPiece* FindPiece(const tstring& strName) const;

private:
	void Init();
	void Destroy();

	bool CreatePieces(const tstring& strFile);
	void DestroyPieces();

private:
	TV_PIECE m_vPiece;

};

#endif // __OEMESH_IMPL_H__
