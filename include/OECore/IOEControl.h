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
#include "IOERenderData.h"

class IOEControl : public IOEObject
{
public:
	IOEControl(uint nType) {m_nType = nType;};
	virtual ~IOEControl() {};

	uint GetType() const {return m_nType;};

	virtual bool Update(IOERenderData* pRenderData, float fDetailTime) = 0;

private:
	uint m_nType;

};
#endif // __IOECONTROL_H__
