/*!
 * \file OESkinMeshRender_Impl.h
 * \date 13-2-2010 19:58:11
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OESKINMESHRENDER_IMPL_H__
#define __OESKINMESHRENDER_IMPL_H__

#include <IOERender.h>
#include "OEBaseTypeEx.h"
#include "OESkinMeshRenderData_Impl.h"

class COESkinMeshRender_Impl : public IOERender
{
public:
	COESkinMeshRender_Impl();
	virtual ~COESkinMeshRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	COESkinMeshRenderData_Impl* ConvertData(IOERenderData* pRenderData);

};
#endif // __OESKINMESHRENDER_IMPL_H__
