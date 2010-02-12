/*!
 * \file OEModelRenderData_Impl.h
 * \date 12-2-2010 20:05:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODELRENDERDATA_IMPL_H__
#define __OEMODELRENDERDATA_IMPL_H__

#include <IOERenderData.h>
#include <IOEMesh.h>
#include <IOEBone.h>
#include <vector>

class COEModelRenderData_Impl : public IOERenderData
{
public:
	typedef std::vector<IOEBone*> TV_BONE;

public:
	COEModelRenderData_Impl();
	virtual ~COEModelRenderData_Impl();

private:
	IOEMesh* m_pMesh;
	TV_BONE m_vBones;

};
#endif // __OEMODELRENDERDATA_IMPL_H__
