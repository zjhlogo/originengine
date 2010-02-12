/*!
 * \file OEModelControl_Impl.h
 * \date 12-2-2010 20:45:56
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODELCONTROL_IMPL_H__
#define __OEMODELCONTROL_IMPL_H__

#include <IOEControl.h>

class COEModelControl_Impl : public IOEControl
{
public:
	COEModelControl_Impl();
	virtual ~COEModelControl_Impl();

	virtual void Update(float fDetailTime);

};
#endif // __OEMODELCONTROL_IMPL_H__
