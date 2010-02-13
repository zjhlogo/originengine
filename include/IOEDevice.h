/*!
 * \file IOEDevice.h
 * \date 24-5-2009 10:44:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEDEVICE_H__
#define __IOEDEVICE_H__

#include "IOEObject.h"
#include "IOEVertDecl.h"

class IOEDevice : public IOEObject
{
public:
	IOEDevice() {};
	virtual ~IOEDevice() {};

	virtual bool CreateDevice() = 0;
	virtual void DestroyDevice() = 0;

	virtual void StartPerform() = 0;
	virtual void EndPerform() = 0;

	virtual float GetFPS() = 0;

	virtual IOEVertDecl* CreateVertDecl(const VERT_DECL_ELEMENT* pElement) = 0;

	virtual bool GetDeviceParam(void* pData, const tstring& strParamName) = 0;
};

extern IOEDevice* g_pOEDevice;

#endif // __IOEDEVICE_H__
