/*!
 * \file AnimationTracker.h
 * \date 14-8-2009 20:17:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __ANIMATIONTRACKER_H__
#define __ANIMATIONTRACKER_H__

#include <IOEObject.h>
#include <IOEMeshBone.h>
#include <OEMath/OEMath.h>
#include <vector>

class CAnimationTracker : public IOEObject
{
public:
	typedef std::vector<CAnimationTracker*> VANIM_TRACKER;

public:
	CAnimationTracker(IOEMeshBone* pBone, CMatrix4x4* pmatTransformed, CAnimationTracker* pParentTracker = NULL);
	virtual ~CAnimationTracker();

	void Update(float fDetailTime);
	const CMatrix4x4& GetTransformedMatrix() const;

private:
	void Init();
	void Destroy();

	bool Create(IOEMeshBone* pBone, CMatrix4x4* pmatTransformed, CAnimationTracker* pParentTracker = NULL);
	void AddChild(CAnimationTracker* pChild);

private:
	IOEMeshBone* m_pBone;
	CAnimationTracker* m_pParentTracker;
	CMatrix4x4* m_pmatTransformed;
	VANIM_TRACKER m_vChildTracker;

};
#endif // __ANIMATIONTRACKER_H__
