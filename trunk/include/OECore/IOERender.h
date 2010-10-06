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
	RTTI_DEF(IOERender, IOEObject);

	IOERender() {};
	virtual ~IOERender() {};

	virtual bool Render(IOERenderData* pRenderData) = 0;

};
#endif // __IOERENDER_H__
