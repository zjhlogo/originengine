/*!
 * \file IOEApp.h
 * \date 23-5-2009 23:52:04
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEAPP_H__
#define __IOEAPP_H__

#include "IOEMgr.h"

class IOEApp : public IOEMgr
{
public:
	IOEApp();
	virtual ~IOEApp();

	virtual void Update(float fDetailTime) = 0;
	virtual void Render(float fDetailTime) = 0;

	void Run();

};

extern IOEApp* g_pOEApp;

#endif // __IOEAPP_H__
