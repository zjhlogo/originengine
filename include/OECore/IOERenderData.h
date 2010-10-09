/*!
 * \file IOERenderData.h
 * \date 12-2-2010 20:00:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDERDATA_H__
#define __IOERENDERDATA_H__

#include "../libOEBase/IOEObject.h"
#include "IOEMesh.h"
#include "IOENode.h"

class IOERenderData : public IOEObject
{
public:
	RTTI_DEF(IOERenderData, IOEObject);

	IOERenderData(IOEObject* pHolder) {m_pHolder = pHolder; m_pNode = NULL;};
	virtual ~IOERenderData() {};

	virtual IOEMesh* GetMesh() = 0;

	void SetNode(IOENode* pNode) {m_pNode = pNode;};
	IOENode* GetNode() {return m_pNode;};

	IOEObject* GetHolder() {return m_pHolder;};

private:
	IOENode* m_pNode;
	IOEObject* m_pHolder;

};
#endif // __IOERENDERDATA_H__
