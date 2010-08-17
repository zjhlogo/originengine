/*!
 * \file IOEModel.h
 * \date 9-8-2009 17:34:58
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMODEL_H__
#define __IOEMODEL_H__

#include "IOEObject.h"

class IOEModel : public IOEObject
{
public:
	IOEModel() {};
	virtual ~IOEModel() {};

	virtual void Update(float fDetailTime) = 0;
	virtual void Render() = 0;

};

#endif // __IOEMODEL_H__
