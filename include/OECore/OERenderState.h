/*!
 * \file OERenderState.h
 * \date 10-28-2010 17:03:26
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OERENDERSTATE_H__
#define __OERENDERSTATE_H__

#include "OEBaseTypeEx.h"
#include "../libOEMath/OEMath.h"

class COERenderState
{
public:
	COERenderState()
	{
		m_bZBuffer = true;
		m_bAlphaTest = false;

		m_bFog = false;
		m_nFogColor = 0xFFFFFFFF;
		m_fFogNear = 0.0f;
		m_fFogFar = 0.0f;

		m_eCullMode = CMT_CCW;
		m_eFillMode = FM_SOLID;

		m_bEnableClipPlane = false;
	};

	COERenderState(const COERenderState& rs)
	{
		m_bZBuffer = rs.m_bZBuffer;
		m_bAlphaTest = rs.m_bAlphaTest;

		m_bFog = rs.m_bFog;
		m_nFogColor = rs.m_nFogColor;
		m_fFogNear = rs.m_fFogNear;
		m_fFogFar = rs.m_fFogFar;

		m_eCullMode = rs.m_eCullMode;
		m_eFillMode = rs.m_eFillMode;
	};

	~COERenderState() {};

public:
	bool m_bZBuffer;
	bool m_bAlphaTest;

	bool m_bFog;
	uint m_nFogColor;
	float m_fFogNear;
	float m_fFogFar;

	CULL_MODE_TYPE m_eCullMode;
	FILL_MODE m_eFillMode;

	bool m_bEnableClipPlane;
	CVector4 m_vClipPlane;

};
#endif // __OERENDERSTATE_H__
