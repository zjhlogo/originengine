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

	virtual IOEVertDecl* CreateVertDecl(const VERT_DECL_ELEMENT* pElement);

	virtual bool GetDeviceParam(void* pData, const tstring& strParamName);

private:
	bool Init();
	void Destroy();

	bool InternalCreateWindow();
	void InternalDestroyWindow();

	bool InternalCreateD3D();
	void InternalDestroyD3D();

	void PerformOnce(float fDetailTime);
	void InitializeD3D();

	void ResetFPS(float fLastTime);
	void CalculateFPS();
	void RenderFPS();

	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_nWindowWidth;
	int m_nWindowHeight;
	tstring m_strClassName;
	tstring m_strWindowName;

	float m_fPrevTime;
	float m_fCurrTime;

	VOED3D_VERTDECL m_vD3DVertDecl;

	// fps
	float m_fMaxFPS;
	float m_fCurrFPS;
	float m_fLastFPSTime;
	int m_nFPSCount;
	IOEUIFont* m_pFontFPS;
	IOEUIString* m_pStringFPS;
};

#endif // __OED3DDEVICE_IMPL_H__
