/*!
 * \file IOEControl.h
 * \date 12-2-2010 19:59:42
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOECONTROL_H__
#define __IOECONTROL_H__

#include "IOEObject.h"

class IOEControl : public IOEObject
{
public:
	IOEControl() {};
	virtual ~IOEControl() {};

	virtual void Update(float fDetailTime) = 0;

};
#endif // __IOECONTROL_H__
