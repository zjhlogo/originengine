/*!
 * \file IOERender.h
 * \date 12-2-2010 19:57:34
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDER_H__
#define __IOERENDER_H__

#include "../libOEBase/IOEObject.h"
#include "IOERenderData.h"

class IOERender : public IOEObject
{
public:
	IOERender(uint nType, const tstring& strClassName):IOEObject(strClassName) {m_nType = nType;};
	virtual ~IOERender() {};

	uint GetType() const {return m_nType;};

	virtual bool Render(IOERenderData* pRenderData) = 0;

private:
	uint m_nType;

};
#endif // __IOERENDER_H__
