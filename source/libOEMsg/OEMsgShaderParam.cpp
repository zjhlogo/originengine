/*!
 * \file OEMsgShaderParam.cpp
 * \date 8-17-2010 21:28:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgShaderParam.h>
#include <libOEMsg/OEMsgList.h>

COEMsgShaderParam::COEMsgShaderParam(IOEShader* pShader, IOEMaterial* pMaterial)
:IOEMsg(OMI_SETUP_SHADER_PARAM)
{
	m_pShader = pShader;
	m_pMaterial = pMaterial;
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

IOEMaterial* COEMsgShaderParam::GetMaterial() const
{
	return m_pMaterial;
}

bool COEMsgShaderParam::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_pShader, sizeof(m_pShader));
	pDBRead->Read(&m_pMaterial, sizeof(m_pMaterial));
	return true;
}

bool COEMsgShaderParam::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_pShader, sizeof(m_pShader));
	pDBWrite->Write(&m_pMaterial, sizeof(m_pMaterial));
	return true;
}
