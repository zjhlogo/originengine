/*!
 * \file OEMsgShaderParam.cpp
 * \date 8-17-2010 21:28:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgShaderParam.h>
#include <libOEMsg/OEMsgList.h>

COEMsgShaderParam::COEMsgShaderParam(IOEShader* pShader)
:IOEMsg(OMI_SETUP_SHADER_PARAM)
{
	m_pShader = pShader;
}

COEMsgShaderParam::COEMsgShaderParam(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	FromBuffer(pDBRead);
}

COEMsgShaderParam::~COEMsgShaderParam()
{
	// TODO: 
}

IOEShader* COEMsgShaderParam::GetShader() const
{
	return m_pShader;
}

bool COEMsgShaderParam::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_pShader, sizeof(m_pShader));
	return true;
}

bool COEMsgShaderParam::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_pShader, sizeof(m_pShader));
	return true;
}
