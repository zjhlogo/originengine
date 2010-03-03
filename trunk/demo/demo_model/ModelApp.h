/*!
 * \file ModelApp.h
 * \date 1-3-2010 21:43:12
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MODELAPP_H__
#define __MODELAPP_H__

#include <IOEApp.h>
#include <IOEModel.h>
#include <vector>

#include "../common/Camera.h"

class CModelApp : public IOEApp
{
public:
	enum CONST_DEFINE
	{
		KEY_COUNT = 256,
	};

public:
	CModelApp();
	~CModelApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);
	virtual void Render3D(float fDetailTime);
	virtual void Render2D(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnLButtonDown(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnLButtonUp(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnMouseMove(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnKeyUp(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnKeyDown(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnSetupShaderParam(uint nMsgID, COEDataBufferRead* pDBRead);

	bool UpdateMovement(float fDetailTime);
	bool UpdateRotation(float fDetailTime);

private:
	CCamera* m_pCamera;
	IOEModel* m_pModel;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};
#endif // __MODELAPP_H__
