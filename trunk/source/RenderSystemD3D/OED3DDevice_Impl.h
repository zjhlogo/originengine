/*!
 * \file OED3DDevice_Impl.h
 * \date 24-5-2009 10:43:46
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DDEVICE_IMPL_H__
#define __OED3DDEVICE_IMPL_H__

#include <OECore/IOEDevice.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "OED3DVertDecl_Impl.h"
#include <vector>

class COED3DDevice_Impl : public IOEDevice
{
public:
	typedef std::vector<COED3DVertDecl_Impl*> TV_OED3D_VERTDECL;

public:
	COED3DDevice_Impl();
	virtual ~COED3DDevice_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void StartPerform();
	virtual void EndPerform();

	virtual float GetCurrTime() const;
	virtual float GetDetailTime() const;

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

	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_nWindowWidth;
	int m_nWindowHeight;
	tstring m_strClassName;
	tstring m_strWindowName;

	float m_fPrevTime;
	float m_fCurrTime;
	float m_fDetailTime;

	TV_OED3D_VERTDECL m_vD3DVertDecl;
	float m_fMaxFPS;

};

#endif // __OED3DDEVICE_IMPL_H__
