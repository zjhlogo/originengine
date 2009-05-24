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

class COED3DDevice_Impl : public IOEDevice
{
public:
	COED3DDevice_Impl();
	virtual ~COED3DDevice_Impl();

	virtual bool CreateDevice();
	virtual void DestroyDevice();

	virtual void StartPerform();
	virtual void EndPerform();

	virtual IOEVertDecl* CreateVertDecl(const IOEVertDecl::ELEMENT* pElement);

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
	const tchar* m_pstrWindowName;

	bool m_bSetTimePeriod;

	float m_fPrevTime;
	float m_fCurrTime;

};

#endif // __OED3DDEVICE_IMPL_H__
