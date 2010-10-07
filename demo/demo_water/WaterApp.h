/*!
 * \file WaterApp.h
 * \date 20-6-2009 9:57:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __WATERAPP_H__
#define __WATERAPP_H__

#include "../common/BaseApp.h"
#include "DlgWaveSetting.h"
#include "Water.h"

class CWaterApp : public CBaseApp
{
public:
	CWaterApp();
	virtual ~CWaterApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	virtual bool OnKeyDown(COEMsgKeyboard& msg);

private:
	CDlgWaveSetting* m_pDlgWaveSetting;
	CWater* m_pWater;

};

#endif // __WATERAPP_H__
