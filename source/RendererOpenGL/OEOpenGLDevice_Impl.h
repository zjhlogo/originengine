/*!
 * \file OEOpenGLDevice_Impl.h
 * \date 24-5-2009 10:57:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEOPENGLDEVICE_IMPL_H__
#define __OEOPENGLDEVICE_IMPL_H__

#include <IOEDevice.h>

class COEOpenGLDevice_Impl : public IOEDevice
{
public:
	COEOpenGLDevice_Impl();
	virtual ~COEOpenGLDevice_Impl();

	virtual bool CreateDevice();
	virtual void DestroyDevice();

	virtual void StartPerform();
	virtual void EndPerform();

	virtual IOEVertDecl* CreateVertDecl(const IOEVertDecl::ELEMENT* pElement);

};

#endif // __OEOPENGLDEVICE_IMPL_H__