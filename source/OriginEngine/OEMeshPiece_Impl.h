/*!
 * \file OEMeshPiece_Impl.h
 * \date 8-8-2009 21:29:42
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMESHPIECE_IMPL_H__
#define __OEMESHPIECE_IMPL_H__

#include <IOEMeshPiece.h>
#include <IOEFile.h>
#include <IOEVertDecl.h>
#include <OEFmtMesh.h>

class COEMeshPiece_Impl : public IOEMeshPiece
{
public:
	COEMeshPiece_Impl(const COEFmtMesh::PIECE& Piece, IOEFile* pFile);
	virtual ~COEMeshPiece_Impl();

	virtual const tstring& GetName() const;
	virtual int GetNumVerts() const;
	virtual int GetNumIndis() const;
	virtual void* GetVerts() const;
	virtual ushort* GetIndis() const;

private:
	void Init();
	void Destroy();

	bool Create(const COEFmtMesh::PIECE& Piece, IOEFile* pFile);
	bool CreateVertDecl(uint nVertexDataMask);

private:
	tstring m_strName;
	IOEVertDecl* m_pDecl;

	int m_nVerts;
	int m_nIndis;
	void* m_pVerts;
	ushort* m_pIndis;

};

#endif // __OEMESHPIECE_IMPL_H__