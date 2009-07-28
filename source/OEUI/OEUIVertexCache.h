/*!
 * \file OEUIVertexCache.h
 * \date 28-7-2009 11:21:14
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUIVERTEXCACHE_H__
#define __OEUIVERTEXCACHE_H__

#include <OEInterfaces.h>

class COEUIVertexCache
{
public:
	COEUIVertexCache(uint nVertsCacheSize, uint nIndisCacheCount);
	~COEUIVertexCache();

	void SetVertDecl(IOEVertDecl* pDecl);
	IOEVertDecl* GetVertDecl() const;

	void SetTexture(IOETexture* pTexture);
	IOETexture* GetTexture() const;

	bool AddVerts(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	void Flush();

	bool Compare(IOEVertDecl* pDecl, IOETexture* pTexture);

	int GetVertsCount();
	int GetIndisCount();

	bool IsOK();

private:
	void Init();
	void Destroy();

	bool Create(uint nVertsCacheSize, uint nIndisCacheCount);

private:
	uint m_nVertsCacheSize;
	uint m_nMaxVertsCount;
	uint m_nMaxIndisCount;
	uint m_nVertsCount;
	uint m_nIndisCount;
	uint m_nStrideSize;

	void* m_pVertsCache;
	ushort* m_pIndisCache;

	IOEVertDecl* m_pDecl;
	IOETexture* m_pTexture;

	bool m_bOK;
};

#endif // __OEUIVERTEXCACHE_H__
