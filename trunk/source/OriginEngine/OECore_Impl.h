/*!
 * \file OECore_Impl.h
 * \date 24-5-2009 9:24:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OECORE_IMPL_H__
#define __OECORE_IMPL_H__

#include <IOECore.h>

class COECore_Impl : public IOECore
{
public:
	COECore_Impl();
	virtual ~COECore_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Run();
	virtual void End();

	virtual void Update();

private:
	bool Init();
	void Destroy();

private:
	bool m_bRunning;

};

#endif // __OECORE_IMPL_H__
