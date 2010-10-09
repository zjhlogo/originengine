/*!
 * \file OEMsgDestroy.cpp
 * \date 10-8-2010 23:09:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgDestroy.h>
#include <libOEMsg/OEMsgList.h>

COEMsgDestroy::COEMsgDestroy(IOEObject* pObject)
:IOEMsg(OMI_OBJECT_DESTROY)
{
	m_pObject = pObject;
}

COEMsgDestroy::COEMsgDestroy(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	FromBuffer(pDBRead);
}

COEMsgDestroy::~COEMsgDestroy()
{
	// TODO: 
}

IOEObject* COEMsgDestroy::GetObjectHandle()
{
	return m_pObject;
}

bool COEMsgDestroy::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_pObject, sizeof(m_pObject));
	return true;
}

bool COEMsgDestroy::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_pObject, sizeof(m_pObject));
	return true;
}
