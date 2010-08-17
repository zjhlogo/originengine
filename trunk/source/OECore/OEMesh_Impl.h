/*!
 * \file OEMesh_Impl.h
 * \date 30-7-2009 16:50:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMESH_IMPL_H__
#define __OEMESH_IMPL_H__

#include <OECore/IOEMesh.h>
#include <OECore/IOEResMgr.h>

class COEMesh_Impl : public IOEMesh
{
public:
	COEMesh_Impl(const tstring& strFile);
	virtual ~COEMesh_Impl();

	virtual int GetNumPieces() const;
	virtual IOEPiece* GetPiece(int nIndex) const;
	virtual IOEPiece* FindPiece(const tstring& strName) const;

	virtual const CVector3& GetBoundingBoxMin() const;
	virtual const CVector3& GetBoundingBoxMax() const;

private:
	void Init();
	void Destroy();

	bool CreatePieces(const tstring& strFile);
	void DestroyPieces();

private:
	TV_PIECE m_vPiece;
	CVector3 m_vBoundingBoxMin;
	CVector3 m_vBoundingBoxMax;

};

#endif // __OEMESH_IMPL_H__
