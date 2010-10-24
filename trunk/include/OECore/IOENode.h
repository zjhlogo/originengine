/*!
 * \file IOENode.h
 * \date 10-4-2010 21:41:34
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOENODE_H__
#define __IOENODE_H__

#include "../libOEBase/IOEObject.h"
#include "../libOEMath/OEMath.h"

class IOENode : public IOEObject
{
public:
	RTTI_DEF(IOENode, IOEObject);

	IOENode() {m_bDirty = true;};
	virtual ~IOENode() {};

	virtual const tstring& GetName() = 0;
	virtual IOENode* NewChildNode(const tstring& strName) = 0;
	virtual bool RemoveChildNode(IOENode* pNode) = 0;
	virtual IOENode* GetChildNode(int nIndex) = 0;
	virtual IOENode* GetChildNode(const tstring& strName) = 0;
	virtual int GetNumChildNodes() = 0;

	virtual bool AttachObject(IOEObject* pObject) = 0;
	virtual bool DettachObject(IOEObject* pObject) = 0;
	virtual IOEObject* GetAttachedObject(int nIndex) = 0;
	virtual int GetNumAttachedObjects() = 0;

	virtual void SetPosition(const CVector3& vPos) = 0;
	virtual const CVector3& GetPosition() = 0;

	virtual void SetRotation(const CQuaternion& qRot) = 0;
	virtual const CQuaternion& GetRotation() = 0;

	virtual void GetLocalMatrix(CMatrix4x4& matLocal) = 0;
	virtual const CMatrix4x4& GetFinalMatrix() = 0;

	virtual void Update(const CMatrix4x4& matParent) = 0;

	virtual bool IsDirty() {return m_bDirty;};
	virtual void SetDirty() {m_bDirty = true;};
	virtual void ClearDirty() {m_bDirty = false;};

private:
	bool m_bDirty;

};
#endif // __IOENODE_H__
