/*!
 * \file OEMsgObjectDestroy.cpp
 * \date 10-24-2010 8:29:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgObjectDestroy.h>
#include <libOEMsg/OEMsgList.h>

COEMsgObjectDestroy::COEMsgObjectDestroy(IOEObject* pObject)
:IOEMsg(OMI_OBJECT_DESTROY)
{
	m_pObject = pObject;
}

COEMsgObjectDestroy::COEMsgObjectDestroy(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	FromBuffer(pDBRead);
}

COEMsgObjectDestroy::~COEMsgObjectDestroy()
{
	// TODO: 
}

IOEObject* COEMsgObjectDestroy::GetObjectHandle()
{
	return m_pObject;
}

bool COEMsgObjectDestroy::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_pObject, sizeof(m_pObject));
	return true;
}

bool COEMsgObjectDestroy::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_pObject, sizeof(m_pObject));
	return true;
}
