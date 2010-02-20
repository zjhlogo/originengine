/*!
 * \file OED3DRenderState.h
 * \date 2-20-2010 13:37:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DRENDERSTATE_H__
#define __OED3DRENDERSTATE_H__

#include <OEBasicType.h>
#include <OEBaseTypeEx.h>

class COED3DRenderState
{
public:
	COED3DRenderState();
	COED3DRenderState(const COED3DRenderState& rs);
	~COED3DRenderState();

public:
	bool m_bZBuffer;
	bool m_bAlphaTest;

	bool m_bFog;
	uint m_nFogColor;
	float m_fFogNear;
	float m_fFogFar;

	CULL_MODE_TYPE m_eCullMode;
	FILL_MODE m_eFillMode;

};

#endif // __OED3DRENDERSTATE_H__
