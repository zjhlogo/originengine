/*!
 * \file OEModelRender_Impl.h
 * \date 12-2-2010 20:21:04
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODELRENDER_IMPL_H__
#define __OEMODELRENDER_IMPL_H__

#include <IOERender.h>
#include "OEBaseTypeEx.h"
#include "OEModelRenderData_Impl.h"

class COEModelRender_Impl : public IOERender
{
public:
	COEModelRender_Impl();
	virtual ~COEModelRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	COEModelRenderData_Impl* ConvertData(IOERenderData* pRenderData);

};
#endif // __OEMODELRENDER_IMPL_H__
