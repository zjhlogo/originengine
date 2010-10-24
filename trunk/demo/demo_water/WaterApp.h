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
#include <libOEMsg/OEMsgKeyboard.h>
#include "DlgWaveSetting.h"
#include "Water.h"

class CWaterApp : public CBaseApp
{
public:
	CWaterApp();
	virtual ~CWaterApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();
	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnKeyDown(COEMsgKeyboard& msg);

private:
	CDlgWaveSetting* m_pDlgWaveSetting;
	CWater* m_pWater;

};

#endif // __WATERAPP_H__
