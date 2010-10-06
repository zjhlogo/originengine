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

	IOENode() {};
	virtual ~IOENode() {};

	virtual bool AddNode(IOENode* pNode) = 0;
	virtual bool RemoveNode(IOENode* pNode) = 0;
	virtual IOENode* GetChildNode(int nIndex) = 0;
	virtual int GetNumChildNodes() = 0;

	virtual bool AttachObject(IOEObject* pObject) = 0;
	virtual bool DettachObject(IOEObject* pObject) = 0;
	virtual IOEObject* GetAttachedObject(int nIndex) = 0;
	virtual int GetNumAttachedObjects() = 0;

	virtual void SetPosition(const CVector3& vPos) = 0;
	virtual void SetRotation(const CQuaternion& qRot) = 0;
	virtual void GetTransform(CMatrix4x4& matTrans) = 0;

};
#endif // __IOENODE_H__
