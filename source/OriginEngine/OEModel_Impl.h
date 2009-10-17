/*!
 * \file OEModel_Impl.h
 * \date 9-8-2009 17:49:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODEL_IMPL_H__
#define __OEMODEL_IMPL_H__

#include <IOEModel.h>

#include <IOEMesh.h>
#include "AnimationTracker.h"

#include <vector>
#include <map>

class COEModel_Impl : public IOEModel
{
public:
	typedef std::map<int, CAnimationTracker*> TRACKER_MAP;
	typedef std::vector<CMatrix4x4> VMATRIX;

public:
	COEModel_Impl(const tstring& strFileName);
	virtual ~COEModel_Impl();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFileName);
	int CalculateBoneCount(IOEMeshBone* pBone);
	CAnimationTracker* LoopCreateTracker(IOEMeshBone* pBone, CAnimationTracker* pParentTracker = NULL);
	CAnimationTracker* CreateTracker(IOEMeshBone* pBone, CAnimationTracker* pParentTracker = NULL);

private:
	IOEMesh* m_pMesh;
	CAnimationTracker* m_pRootTracker;
	TRACKER_MAP m_vTrackerMap;
	VMATRIX m_vmatTransformed;

};

#endif // __OEMODEL_IMPL_H__
