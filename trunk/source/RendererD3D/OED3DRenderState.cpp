/*!
 * \file OED3DRenderState.cpp
 * \date 2-20-2010 13:38:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DRenderState.h"

COED3DRenderState::COED3DRenderState()
{
	m_bZBuffer = true;

	m_bFog = false;
	m_nFogColor = 0xFFFFFFFF;
	m_fFogNear = 0.0f;
	m_fFogFar = 0.0f;

	m_eCullMode = CMT_CCW;
	m_eFillMode = FM_SOLID;
}

COED3DRenderState::COED3DRenderState(const COED3DRenderState& rs)
{
	m_bZBuffer = rs.m_bZBuffer;

	m_bFog = rs.m_bFog;
	m_nFogColor = rs.m_nFogColor;
	m_fFogNear = rs.m_fFogNear;
	m_fFogFar = rs.m_fFogFar;

	m_eCullMode = rs.m_eCullMode;
	m_eFillMode = rs.m_eFillMode;
}

COED3DRenderState::~COED3DRenderState()
{
	// TODO: 
}
