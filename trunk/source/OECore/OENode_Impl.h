/*!
 * \file OENode_Impl.h
 * \date 10-4-2010 21:44:48
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OENODE_IMPL_H__
#define __OENODE_IMPL_H__

#include <OECore/IOENode.h>
#include <vector>

class COENode_Impl : public IOENode
{
public:
	typedef std::vector<IOENode*> TV_NODE;
	typedef std::vector<IOEObject*> TV_OBJECT;

public:
	RTTI_DEF(COENode_Impl, IOENode);

	COENode_Impl();
	virtual ~COENode_Impl();

	virtual bool AddNode(IOENode* pNode);
	virtual bool RemoveNode(IOENode* pNode);
	virtual IOENode* GetChildNode(int nIndex);
	virtual int GetNumChildNodes();

	virtual bool AttachObject(IOEObject* pObject);
	virtual bool DettachObject(IOEObject* pObject);
	virtual IOEObject* GetAttachedObject(int nIndex);
	virtual int GetNumAttachedObjects();

	virtual void SetPosition(const CVector3& vPos);
	virtual const CVector3& GetPosition();

	virtual void SetRotation(const CQuaternion& qRot);
	virtual const CQuaternion& GetRotation();

	virtual void Update(const CMatrix4x4& matParent);

	virtual void GetLocalMatrix(CMatrix4x4& matLocal);
	virtual const CMatrix4x4& GetFinalMatrix();

private:
	TV_NODE m_vNodes;
	TV_OBJECT m_vObjects;

	CVector3 m_vPos;
	CQuaternion m_qRot;

	CMatrix4x4 m_matFinal;

};
#endif // __OENODE_IMPL_H__
