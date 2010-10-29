/*!
 * \file OEMsgRenderState.cpp
 * \date 10-29-2010 0:03:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgRenderState.h>
#include <libOEMsg/OEMsgList.h>

COEMsgRenderState::COEMsgRenderState(const COERenderState& RenderState, const tstring& strCommon /* = EMPTY_STRING */)
:IOEMsg(OMI_RESTORE_RENDER_STATE)
{
	m_RenderState = RenderState;
	m_strCommon = strCommon;
}

COEMsgRenderState::COEMsgRenderState(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	FromBuffer(pDBRead);
}

COEMsgRenderState::~COEMsgRenderState()
{
	// TODO: 
}

bool COEMsgRenderState::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_RenderState, sizeof(m_RenderState));
	// TODO: read string
	return true;
}

bool COEMsgRenderState::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_RenderState, sizeof(m_RenderState));
	// TODO: write string
	return true;
}

COERenderState& COEMsgRenderState::GetRenderState()
{
	return m_RenderState;
}

const tstring& COEMsgRenderState::GetCommon()
{
	return m_strCommon;
}
