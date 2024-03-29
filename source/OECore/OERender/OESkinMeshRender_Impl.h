/*!
 * \file OESkinMeshRender_Impl.h
 * \date 13-2-2010 19:58:11
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OESKINMESHRENDER_IMPL_H__
#define __OESKINMESHRENDER_IMPL_H__

#include <OECore/OEBaseTypeEx.h>
#include <OECore/IOERender.h>

class COESkinMeshRender_Impl : public IOERender
{
public:
	RTTI_DEF(COESkinMeshRender_Impl, IOERender);

	COESkinMeshRender_Impl();
	virtual ~COESkinMeshRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

};
#endif // __OESKINMESHRENDER_IMPL_H__
