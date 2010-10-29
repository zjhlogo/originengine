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
#include "IOESkeleton.h"
#include "IOEMaterial.h"
#include "IOENode.h"

class IOERenderData : public IOEObject
{
public:
	RTTI_DEF(IOERenderData, IOEObject);

	IOERenderData(IOEObject* pHolder) {m_pHolder = pHolder; m_pNode = NULL; m_pRenderTarget = NULL;};
	virtual ~IOERenderData() {};

	void SetNode(IOENode* pNode) {m_pNode = pNode;};
	IOENode* GetNode() {return m_pNode;};
	IOEObject* GetHolder() {return m_pHolder;};

	virtual IOEMesh* GetMesh() = 0;

	virtual IOESkeleton* GetSkeleton() = 0;
	virtual TV_MATRIX4X4& GetSkinMatrix() = 0;
	virtual IOEMaterial* GetMaterial(int nIndex) = 0;

	virtual void SetAnimLength(float fAnimLength) = 0;
	virtual float GetAnimLength() const = 0;

	virtual void SetTotalTime(float fTotalTime) = 0;
	virtual float GetTotalTime() const = 0;

	virtual void SetRenderTarget(IOETexture* pTexture) {m_pRenderTarget = pTexture;};
	virtual IOETexture* GetRenderTarget() {return m_pRenderTarget;};

private:
	IOENode* m_pNode;
	IOEObject* m_pHolder;
	IOETexture* m_pRenderTarget;

};
#endif // __IOERENDERDATA_H__
