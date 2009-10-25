/*!
 * \file OEPiece_Impl.h
 * \date 10-23-2009 13:59:49
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEPIECE_IMPL_H__
#define __OEPIECE_IMPL_H__

#include <IOEPiece.h>
#include <IOEFile.h>
#include <OEFmtMesh.h>

#include <vector>

class COEPiece_Impl : public IOEPiece
{
public:
	COEPiece_Impl(const COEFmtMesh::PIECE& Piece, IOEFile* pFile);
	virtual ~COEPiece_Impl();

	virtual const tstring& GetName() const;

	virtual int GetMaterialID() const;
	virtual int GetVertDecl() const;

	virtual int GetNumVerts() const;
	virtual int GetNumIndis() const;
	virtual void* GetVerts() const;
	virtual ushort* GetIndis() const;

private:
	void Init();
	void Destroy();

	bool Create(const COEFmtMesh::PIECE& Piece, IOEFile* pFile);

private:
	tstring m_strName;
	int m_nMaterialID;
	int m_nVertDecl;

	int m_nVerts;
	int m_nIndis;
	void* m_pVerts;
	ushort* m_pIndis;

};
#endif // __OEPIECE_IMPL_H__
