/*!
 * \file OED3DDevice_Impl.cpp
 * \date 24-5-2009 10:44:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DDevice_Impl.h"

#include <OEMath/OEMath.h>
#include <OEInterfaces.h>

IDirect3D9* g_pD3D = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;
HWND g_hWnd = NULL;

COED3DDevice_Impl::COED3DDevice_Impl()
{
	g_pOEDevice = this;
	Init();
}

COED3DDevice_Impl::~COED3DDevice_Impl()
{
	Destroy();
	g_pOEDevice = NULL;
}

void COED3DDevice_Impl::Init()
{
	// TODO: read from config file
	m_nWindowWidth = 800;
	m_nWindowHeight = 600;
	m_pstrWindowName = _T("Origin Engine");

	m_bSetTimePeriod = false;
	m_fPrevTime = 0.0f;
	m_fCurrTime = 0.0f;
	m_vD3DVertDecl.clear();
}

void COED3DDevice_Impl::Destroy()
{
	DestroyDevice();
}

bool COED3DDevice_Impl::CreateDevice()
{
	// create window
	if (!InternalCreateWindow())
	{
		DestroyDevice();
		return false;
	}

	// create direct3d
	if (!InternalCreateD3D())
	{
		DestroyDevice();
		return false;
	}

	// initialize
	InitializeD3D();

	return true;
}

void COED3DDevice_Impl::DestroyDevice()
{
	if (g_pd3dDevice) InternalDestroyD3D();
	if (g_hWnd) InternalDestroyWindow();
}

void COED3DDevice_Impl::StartPerform()
{
	// show window
	ShowWindow(g_hWnd, SW_NORMAL);
	UpdateWindow(g_hWnd);

	// TODO: logout start perform

	m_fPrevTime = (float)timeGetTime()/1000.0f;
	m_fCurrTime = m_fPrevTime;

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_fCurrTime = (float)timeGetTime()/1000.0f;
			PerformOnce(m_fCurrTime - m_fPrevTime);
			m_fPrevTime = m_fCurrTime;
		}
	}
}

void COED3DDevice_Impl::EndPerform()
{
	PostQuitMessage(0);
}

IOEVertDecl* COED3DDevice_Impl::CreateVertDecl(const IOEVertDecl::ELEMENT* pElement)
{
	for (VOED3D_VERTDECL::iterator it = m_vD3DVertDecl.begin(); it != m_vD3DVertDecl.end(); ++it)
	{
		COED3DVertDecl_Impl* pDecl = (*it);
		if (pDecl->Compare(pElement))
		{
			pDecl->IncRef();
			return pDecl;
		}
	}

	COED3DVertDecl_Impl* pDecl = new COED3DVertDecl_Impl(pElement);
	if (!pDecl || !pDecl->IsOK())
	{
		SAFE_DELETE(pDecl);
		// TODO: logout failed
		return NULL;
	}

	m_vD3DVertDecl.push_back(pDecl);

	return pDecl;
}

bool COED3DDevice_Impl::GetDeviceParam(void* pData, const tchar* pstrParamName)
{
	if (_tcscmp(pstrParamName, _T("HWND")) == 0)
	{
		*(HWND*)pData = g_hWnd;
		return true;
	}

	return false;
}

bool COED3DDevice_Impl::InternalCreateWindow()
{
	if (g_hWnd) InternalDestroyWindow();

	HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASSEX wc = {sizeof(WNDCLASSEX),	// Specifies the size of this structure
		CS_OWNDC,							// Specifies the class style
		COED3DDevice_Impl::MainWndProc,		// Pointer to the window procedure
		0,									// Specifies the number of extra bytes to allocate following the window-class structure
		0,									// Specifies the number of extra bytes to allocate following the window instance
		hInst,								// Handle to the instance
		LoadIcon(NULL, IDI_APPLICATION),	// Handle to the class icon
		LoadCursor(NULL, IDC_ARROW),		// Handle to the class cursor
		(HBRUSH)COLOR_WINDOW,				// Handle to the class background brush
		NULL,								// resource name of the class menu
		_T("OriginEngine"),					// Pointer to a null-terminated string or is an atom
		LoadIcon(NULL, IDI_APPLICATION)};	// Handle to a small icon that is associated with the window class
	RegisterClassEx(&wc);

	// screen width, height
	uint nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	uint nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	RECT rc = {0, 0, m_nWindowWidth, m_nWindowHeight};
	AdjustWindowRect(&rc, dwStyle, FALSE);
	uint nAdjustWidth = rc.right - rc.left;
	uint nAdjustHeight = rc.bottom - rc.top;

	// create the window
	g_hWnd = CreateWindow(_T("OriginEngine"),
		m_pstrWindowName,
		dwStyle,
		(nScreenWidth-nAdjustWidth)/2,
		(nScreenHeight-nAdjustHeight)/2,
		nAdjustWidth,
		nAdjustHeight,
		NULL,
		NULL,
		hInst,
		NULL);
	if (!g_hWnd)
	{
		// TODO: logout
		return false;
	}

	// TODO: logout ok
	return true;
}

void COED3DDevice_Impl::InternalDestroyWindow()
{
	if (g_hWnd)
	{
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
	}

	// TODO: logout
}

bool COED3DDevice_Impl::InternalCreateD3D()
{
	if (g_pd3dDevice) InternalDestroyD3D();

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		// TODO: logout failed
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	uint nAdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE eDeviceType = D3DDEVTYPE_HAL;

	//for (uint nAdapter = 0; nAdapter < g_pD3D->GetAdapterCount(); ++nAdapter)
	//{
	//	D3DADAPTER_IDENTIFIER9 Identifier;
	//	HRESULT hRes = g_pD3D->GetAdapterIdentifier(nAdapter, 0, &Identifier);
	//	if (strstr(Identifier.Description, "PerfHUD") != 0)
	//	{
	//		nAdapterToUse = nAdapter;
	//		eDeviceType = D3DDEVTYPE_REF;
	//		break;
	//	}
	//}

	if (FAILED(g_pD3D->CreateDevice(nAdapterToUse,
		eDeviceType,
		g_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pd3dDevice)))
	{
		// TODO: logout failed
		InternalDestroyD3D();
		return false;
	}

	timeBeginPeriod(1);
	m_bSetTimePeriod = true;

	// TODO: logout ok

	return true;
}

void COED3DDevice_Impl::InternalDestroyD3D()
{
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pD3D);

	if (m_bSetTimePeriod)
	{
		timeEndPeriod(1);
		m_bSetTimePeriod = false;
	}

	// TODO: logout

	// TODO: destroy m_vD3DVertDecl vector
}

void COED3DDevice_Impl::PerformOnce(float fDetailTime)
{
	g_pOEApp->Update(fDetailTime);

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		g_pOEApp->Render(fDetailTime);

		g_pd3dDevice->EndScene();
		g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}
}

LRESULT CALLBACK COED3DDevice_Impl::MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static short s_nLastMousePosX = 0;
	static short s_nLastMousePosY = 0;

	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			SetCapture(hWnd);
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			g_pOEApp->OnLButtonDown(x, y);
		}
		break;
	case WM_LBUTTONUP:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			g_pOEApp->OnLButtonUp(x, y);
			ReleaseCapture();
		}
		break;
	case WM_MOUSEMOVE:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			g_pOEApp->OnMouseMove(x-s_nLastMousePosX, y-s_nLastMousePosY);
			s_nLastMousePosX = x;
			s_nLastMousePosY = y;
		}
		break;
	case WM_KEYUP:
		{
			g_pOEApp->OnKeyUp((int)wParam);
		}
		break;
	case WM_KEYDOWN:
		{
			g_pOEApp->OnKeyDown((int)wParam);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	}

	return 0;
}

void COED3DDevice_Impl::InitializeD3D()
{
	// initialize
	CMatrix4x4 matWorld;
	matWorld.Identity();
	g_pOERenderer->SetTransform(IOERenderer::TT_WORLD, matWorld);

	CMatrix4x4 matView;
	COEMath::BuildLookAtMatrixLH(matView, CVector3(0.0f, 3.0f, -5.0f), CVector3(0.0f, 0.0f, 0.0f), CVector3(0.0f, 1.0f, 0.0f));
	g_pOERenderer->SetTransform(IOERenderer::TT_VIEW, matView);

	CMatrix4x4 matProj;
	COEMath::BuildProjectMatrixLH(matProj, OEMATH_PI/4.0f, (float)m_nWindowWidth/(float)m_nWindowHeight, 1.0f, 10000.0f);
	g_pOERenderer->SetTransform(IOERenderer::TT_PROJECTION, matProj);

	g_pOERenderer->EnableLight(false);

	// TODO: integrate into g_pOERenderer
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}