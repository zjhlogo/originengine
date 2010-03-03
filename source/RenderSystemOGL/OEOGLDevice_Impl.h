/*!
 * \file OEOGLDevice_Impl.h
 * \date 3-3-2010 11:05:14
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEOGLDEVICE_IMPL_H__
#define __OEOGLDEVICE_IMPL_H__

#include <IOEDevice.h>

class COEOGLevice_Impl : public IOEDevice
{
public:
	COEOGLevice_Impl();
	virtual ~COEOGLevice_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void StartPerform();
	virtual void EndPerform();

	virtual float GetCurrTime() const;
	virtual float GetDetailTime() const;

	virtual IOEVertDecl* CreateVertDecl(const VERT_DECL_ELEMENT* pElement);

	virtual bool GetDeviceParam(void* pData, const tstring& strParamName);

private:
	bool Init();
	void Destroy();

};
#endif // __OEOGLDEVICE_IMPL_H__
