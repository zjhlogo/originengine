/*!
 * \file OED3DVertDecl_Impl.h
 * \date 24-5-2009 17:47:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DVERTDECL_IMPL_H__
#define __OED3DVERTDECL_IMPL_H__

#include <OECore/IOEVertDecl.h>
#include <d3d9.h>

class COED3DVertDecl_Impl : public IOEVertDecl
{
public:
	COED3DVertDecl_Impl(const VERT_DECL_ELEMENT* pElement);
	virtual ~COED3DVertDecl_Impl();

	virtual int GetStrideSize() const;

	const VERT_DECL_ELEMENT* GetElement() const;
	bool Compare(const VERT_DECL_ELEMENT* pElement) const;

	IDirect3DVertexDeclaration9* GetD3DVertDecl();

private:
	void Init();
	void Destroy();

	bool Create(const VERT_DECL_ELEMENT* pElement);

private:
	VERT_DECL_ELEMENT m_Element[MAX_ELEMENT_COUNT];
	uint m_nElementCount;
	uint m_nStride;
	IDirect3DVertexDeclaration9* m_pDecl;

};
#endif // __OED3DVERTDECL_IMPL_H__
