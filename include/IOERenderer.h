/*!
 * \file IOERenderer.h
 * \date 24-5-2009 17:14:26
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDERER_H__
#define __IOERENDERER_H__

#include "OEBasicType.h"
#include "IOEVertDecl.h"

class IOERenderer
{
public:
	IOERenderer() {};
	virtual ~IOERenderer() {};

	virtual void SetVertDecl(IOEVertDecl* pDecl) = 0;
	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
};

extern IOERenderer* g_pOERenderer;

#endif // __IOERENDERER_H__
