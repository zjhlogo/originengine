/*!
 * \file OERenderSystemUtil.h
 * \date 20-2-2010 19:34:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OERENDERSYSTEMUTIL_H__
#define __OERENDERSYSTEMUTIL_H__

#include "IOERenderSystem.h"

class CDefaultRenderState
{
public:
	CDefaultRenderState()
	{
		g_pOERenderSystem->PushRenderState();
		g_pOERenderSystem->RestoreRenderState();
	};

	~CDefaultRenderState()
	{
		g_pOERenderSystem->PopRenderState();
	};
};
#endif // __OERENDERSYSTEMUTIL_H__
