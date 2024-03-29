/*!
 * \file IOEControl.h
 * \date 12-2-2010 19:59:42
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOECONTROL_H__
#define __IOECONTROL_H__

#include "../libOEBase/IOEObject.h"
#include "IOERenderData.h"

class IOEControl : public IOEObject
{
public:
	RTTI_DEF(IOEControl, IOEObject);

	IOEControl() {};
	virtual ~IOEControl() {};

	virtual bool Update(IOERenderData* pRenderData, float fDetailTime) = 0;

};
#endif // __IOECONTROL_H__
