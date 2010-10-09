/*!
 * \file OENode_Impl.cpp
 * \date 10-4-2010 21:45:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OENode_Impl.h"
#include <libOEMsg/OEMsgList.h>

COENode_Impl::COENode_Impl(const tstring& strName)
{
	m_strName = strName;
}

COENode_Impl::~COENode_Impl()
{
	DestroyChildren();
}

const tstring& COENode_Impl::GetName()
{
	return m_strName;
}

IOENode* COENode_Impl::NewChildNode(const tstring& strName)
{
	// check if exist the name
	for (TV_NODE::iterator it = m_vNodes.begin(); it != m_vNodes.end(); ++it)
	{
		if ((*it)->GetName() == strName) return NULL;
	}

	COENode_Impl* pNode = new COENode_Impl(strName);
	if (!pNode) return NULL;

	m_vNodes.push_back(pNode);
	return pNode;
}

bool COENode_Impl::RemoveChildNode(IOENode* pNode)
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

	pObject->RegisterEvent(OMI_OBJECT_DESTROY, this, (MSG_FUNC)&COENode_Impl::OnObjectDestroy);
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
	SetDirty();
}

const CVector3& COENode_Impl::GetPosition()
{
	return m_vPos;
}

void COENode_Impl::SetRotation(const CQuaternion& qRot)
{
	m_qRot = qRot;
	SetDirty();
}

const CQuaternion& COENode_Impl::GetRotation()
{
	return m_qRot;
}

void COENode_Impl::Update(const CMatrix4x4& matParent)
{
	CMatrix4x4 matLocal;
	GetLocalMatrix(matLocal);
	m_matFinal = matLocal*matParent;
}

void COENode_Impl::GetLocalMatrix(CMatrix4x4& matLocal)
{
	COEMath::BuildMatrixFromQuaternion(matLocal, m_qRot);
	COEMath::SetMatrixTranslation(matLocal, m_vPos);
}

const CMatrix4x4& COENode_Impl::GetFinalMatrix()
{
	return m_matFinal;
}

void COENode_Impl::DestroyChildren()
{
	for (TV_NODE::iterator it = m_vNodes.begin(); it != m_vNodes.end(); ++it)
	{
		COENode_Impl* pNode = (*it);
		pNode->DestroyChildren();
		SAFE_RELEASE(pNode);
	}

	m_vNodes.clear();
}

bool COENode_Impl::OnObjectDestroy(COEMsgDestroy& msg)
{
	DettachObject(msg.GetObjectHandle());
	return true;
}
