/*!
 * \file IOEUIRenderer.h
 * \date 27-7-2009 17:47:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEUIRENDERER_H__
#define __IOEUIRENDERER_H__

#include "OEUIType.h"
#include "../IOEVertDecl.h"
#include "../IOETexture.h"

class IOEUIRenderer
{
public:
	IOEUIRenderer() {};
	virtual ~IOEUIRenderer() {};

	virtual void SetTexture(IOETexture* pTexture) = 0;
	virtual IOETexture* GetTexture() const = 0;

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void FlushAll() = 0;
};

extern IOEUIRenderer* g_pOEUIRenderer;

#endif // __IOEUIRENDERER_H__
