/*!
 * \file IOEApp.h
 * \date 23-5-2009 23:52:04
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEAPP_H__
#define __IOEAPP_H__

#include "OEBasicType.h"

class IOEApp
{
public:
	IOEApp();
	virtual ~IOEApp();

	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

	virtual void Update(float fDetailTime) = 0;
	virtual void Render(float fDetailTime) = 0;

	void Run();

};

extern IOEApp* g_pOEApp;

#endif // __IOEAPP_H__
