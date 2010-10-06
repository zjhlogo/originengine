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
#include <libOEMsg/OEMsgCommand.h>
#include <vector>
#include "OEFPSPrinter.h"

class COECore_Impl : public IOECore
{
public:
	RTTI_DEF(COECore_Impl, IOECore);

	COECore_Impl();
	virtual ~COECore_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Run();
	virtual void End();

	virtual IOENode* CreateNewNode();
	virtual IOENode* GetRootNode();

private:
	bool Init();
	void Destroy();

	bool InitializeInterfaces();
	void TerminateInterfaces();

	void RegisterMessage();

	bool UpdateNodes(IOENode* pNode, float fDetailTime);
	bool RenderNodes(IOENode* pNode, float fDetailTime);

	bool OnStartPerform(COEMsgCommand& msg);
	bool OnPreUpdate(COEMsgCommand& msg);
	bool OnUpdate(COEMsgCommand& msg);
	bool OnPostUpdate(COEMsgCommand& msg);
	bool OnPreRender(COEMsgCommand& msg);
	bool OnRender(COEMsgCommand& msg);
	bool OnPostRender(COEMsgCommand& msg);

private:
	bool m_bRunning;
	IOENode* m_pRootNode;
	COEFPSPrinter* m_pFPSPrinter;

};

#endif // __OECORE_IMPL_H__
