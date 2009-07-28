/*!
 * \file OED3DDevice_Impl.h
 * \date 24-5-2009 10:43:46
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DDEVICE_IMPL_H__
#define __OED3DDEVICE_IMPL_H__

#include <IOEDevice.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "OED3DVertDecl_Impl.h"
#include <OEUI/OEUI.h>
#include <vector>

class COED3DDevice_Impl : public IOEDevice
{
public:
	typedef std::vector<COED3DVertDecl_Impl*> VOED3D_VERTDECL;

public:
	COED3DDevice_Impl();
	virtual ~COED3DDevice_Impl();

	virtual bool CreateDevice();
	virtual void DestroyDevice();

	virtual void StartPerform();
	virtual void EndPerform();

	virtual float GetFPS();

	virtual IOEVertDecl* CreateVertDecl(const IOEVertDecl::ELEMENT* pElement);

	virtual bool GetDeviceParam(void* pData, const tstring& strParamName);

private:
	void Init();
	void Destroy();

	bool InternalCreateWindow();
	void InternalDestroyWindow();

	bool InternalCreateD3D();
	void InternalDestroyD3D();

	void PerformOnce(float fDetailTime);
	void InitializeD3D();

	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_nWindowWidth;
	int m_nWindowHeight;
	tstring m_strWindowName;

	float m_fMaxFPS;
	float m_fCurrFPS;
	float m_fLastFPSTime;
	int m_nFPSCount;

	float m_fPrevTime;
	float m_fCurrTime;

	VOED3D_VERTDECL m_vD3DVertDecl;

	IOEUIFont* m_pFontFPS;
	IOEUIString* m_pStringFPS;
};

#endif // __OED3DDEVICE_IMPL_H__
