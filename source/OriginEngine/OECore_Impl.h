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
#include <OEUI/OEUI.h>

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

	void CalculateFPS();

	bool OnStartPerform(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnPreUpdate(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnUpdate(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnPostUpdate(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnPreRender3D(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnRender3D(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnPostRender3D(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnPreRender2D(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnRender2D(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnPostRender2D(uint nMsgID, COEDataBufferRead* pDBRead);

private:
	bool m_bRunning;

	float m_fCurrFPS;
	float m_fLastFPSTime;
	int m_nFPSCount;
	IOEUIFont* m_pFontFPS;
	IOEUIString* m_pStringFPS;

};

#endif // __OECORE_IMPL_H__
