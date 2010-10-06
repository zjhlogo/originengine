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

	IOERenderData() {m_pNode = NULL;};
	virtual ~IOERenderData() {};

	virtual void SetNode(IOENode* pNode) {m_pNode = pNode;};
	virtual IOENode* GetNode() {return m_pNode;};

	virtual IOEMesh* GetMesh() = 0;

private:
	IOENode* m_pNode;

};
#endif // __IOERENDERDATA_H__
