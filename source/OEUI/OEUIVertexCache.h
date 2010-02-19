/*!
 * \file OEUIVertexCache.h
 * \date 28-7-2009 11:21:14
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUIVERTEXCACHE_H__
#define __OEUIVERTEXCACHE_H__

#include <IOEVertDecl.h>
#include <IOETexture.h>

class COEUIVertexCache
{
public:
	COEUIVertexCache(uint nVertsCacheSize, uint nIndisCacheCount);
	~COEUIVertexCache();

	void SetTexture(IOETexture* pTexture);
	IOETexture* GetTexture() const;

	void SetShader(IOEShader* pShader);
	IOEShader* GetShader() const;

	bool AddVerts(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	void Flush();

	bool Compare(IOETexture* pTexture, IOEShader* pShader);

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

	IOETexture* m_pTexture;
	IOEShader* m_pShader;

	bool m_bOK;
};

#endif // __OEUIVERTEXCACHE_H__
