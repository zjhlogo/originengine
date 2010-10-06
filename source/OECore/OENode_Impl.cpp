/*!
 * \file OENode_Impl.cpp
 * \date 10-4-2010 21:45:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OENode_Impl.h"

COENode_Impl::COENode_Impl()
{
	// TODO: 
}

COENode_Impl::~COENode_Impl()
{
	// TODO: 
}

bool COENode_Impl::AddNode(IOENode* pNode)
{
	if (!pNode) return false;

	// check if exist
	for (TV_NODE::iterator it = m_vNodes.begin(); it != m_vNodes.end(); ++it)
	{
		if (*it == pNode) return false;
	}

	m_vNodes.push_back(pNode);
	return true;
}

bool COENode_Impl::RemoveNode(IOENode* pNode)
{
	if (!pNode) return false;

	// find it and remove it
	for (TV_NODE::iterator it = m_vNodes.begin(); it != m_vNodes.end(); ++it)
	{
		if (*it == pNode)
		{
			m_vNodes.erase(it);
			return true;
		}
	}

	return false;
}

IOENode* COENode_Impl::GetChildNode(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)m_vNodes.size()) return NULL;
	return m_vNodes[nIndex];
}

int COENode_Impl::GetNumChildNodes()
{
	return (int)m_vNodes.size();
}

bool COENode_Impl::AttachObject(IOEObject* pObject)
{
	if (!pObject) return false;

	// check if exist
	for (TV_OBJECT::iterator it = m_vObjects.begin(); it != m_vObjects.end(); ++it)
	{
		if (*it == pObject) return false;
	}

	m_vObjects.push_back(pObject);
	return true;
}

bool COENode_Impl::DettachObject(IOEObject* pObject)
{
	if (!pObject) return false;

	// find it and remove it
	for (TV_OBJECT::iterator it = m_vObjects.begin(); it != m_vObjects.end(); ++it)
	{
		if (*it == pObject)
		{
			m_vObjects.erase(it);
			return true;
		}
	}

	return false;
}

IOEObject* COENode_Impl::GetAttachedObject(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)m_vObjects.size()) return NULL;
	return m_vObjects[nIndex];
}

int COENode_Impl::GetNumAttachedObjects()
{
	return (int)m_vObjects.size();
}

void COENode_Impl::SetPosition(const CVector3& vPos)
{
	m_vPos = vPos;
}

void COENode_Impl::SetRotation(const CQuaternion& qRot)
{
	m_qRot = qRot;
}

void COENode_Impl::GetTransform(CMatrix4x4& matTrans)
{
	COEMath::BuildMatrixFromQuaternion(matTrans, m_qRot);
	COEMath::SetMatrixTranslation(matTrans, m_vPos);
}
