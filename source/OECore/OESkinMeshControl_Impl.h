/*!
 * \file OESkinMeshControl_Impl.h
 * \date 13-2-2010 19:56:07
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OESKINMESHCONTROL_IMPL_H__
#define __OESKINMESHCONTROL_IMPL_H__

#include <OECore/IOEControl.h>
#include "OESkinMeshRenderData_Impl.h"

class COESkinMeshControl_Impl : public IOEControl
{
public:
	RTTI_DEF(COESkinMeshControl_Impl, IOEControl);

	COESkinMeshControl_Impl();
	virtual ~COESkinMeshControl_Impl();

	virtual bool Update(IOERenderData* pRenderData, float fDetailTime);

private:
	COESkinMeshRenderData_Impl* ConvertData(IOERenderData* pRenderData);

};
#endif // __OESKINMESHCONTROL_IMPL_H__
