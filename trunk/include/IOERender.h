/*!
 * \file IOERender.h
 * \date 12-2-2010 19:57:34
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDER_H__
#define __IOERENDER_H__

#include "IOEObject.h"

class IOERender : public IOEObject
{
public:
	IOERender() {};
	virtual ~IOERender() {};

	virtual void Render() = 0;

};
#endif // __IOERENDER_H__
