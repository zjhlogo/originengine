/*!
 * \file OECore_Impl.h
 * \date 24-5-2009 9:24:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OECORE_IMPL_H__
#define __OECORE_IMPL_H__

#include <OECore/IOECore.h>
#include <OEUI/IOEUIFontMgr.h>
#include <OEUI/IOEUIStringMgr.h>
#include <libOEMsg/OEMsgCommand.h>

class COECore_Impl : public IOECore
{
public:
	COECore_Impl();
	virtual ~COECore_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Run();
	virtual void End();

private:
	bool Init();
	void Destroy();

	bool InitializeInterfaces();
	void TerminateInterfaces();

	void RegisterMessage();

	void CalculateFPS();

	bool OnStartPerform(COEMsgCommand& msg);
	bool OnPreUpdate(COEMsgCommand& msg);
	bool OnUpdate(COEMsgCommand& msg);
	bool OnPostUpdate(COEMsgCommand& msg);
	bool OnPreRender(COEMsgCommand& msg);
	bool OnRender(COEMsgCommand& msg);
	bool OnPostRender(COEMsgCommand& msg);

private:
	bool m_bRunning;

	float m_fCurrFPS;
	float m_fLastFPSTime;
	int m_nFPSCount;
	IOEUIFont* m_pFontFPS;
	IOEUIString* m_pStringFPS;

};

#endif // __OECORE_IMPL_H__
