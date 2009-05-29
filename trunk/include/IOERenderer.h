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
#include <OEMath/OEMath.h>

class IOERenderer
{
public:
	enum TRANSFORM_TYPE
	{
		TT_UNKNOWN = 0,
		TT_WORLD,
		TT_VIEW,
		TT_PROJECTION,
		TT_MAX,
	};

	enum CULL_MODE_TYPE
	{
		CMT_UNKNOWN = 0,
		CMT_NONE,
		CMT_CW,
		CMT_CCW,
		CMT_MAX,
	};

	enum FILL_MODE
	{
		FM_UNKNOWN = 0,
		FM_POINT,
		FM_WIREFRAME,
		FM_SOLID,
		FM_MAX,
	};

public:
	IOERenderer() {};
	virtual ~IOERenderer() {};

	virtual void SetVertDecl(IOEVertDecl* pDecl) = 0;
	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

	virtual void SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat) = 0;
	virtual void GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) const = 0;

	virtual void EnableLight(bool bEnable) = 0;
	virtual void EnableZBuffer(bool bEnable) = 0;
	virtual void EnableFog(bool bEnable) = 0;

	virtual void SetCullMode(CULL_MODE_TYPE eMode) = 0;
	virtual void SetFillMode(FILL_MODE eFillMode) = 0;
	virtual void SetFogInfo(uint nColor, float fNear, float fFar) = 0;
};

extern IOERenderer* g_pOERenderer;

#endif // __IOERENDERER_H__
