/*!
 * \file IOEMesh.h
 * \date 30-7-2009 10:04:53
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMESH_H__
#define __IOEMESH_H__

#include "IOEObject.h"

class IOEMesh : public IOEObject
{
public:
	IOEMesh() {};
	virtual ~IOEMesh() {};

	virtual void Render(float fDetailTime) = 0;

};
#endif // __IOEMESH_H__
