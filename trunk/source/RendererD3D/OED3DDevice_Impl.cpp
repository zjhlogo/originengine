/*!
 * \file OED3DDevice_Impl.cpp
 * \date 24-5-2009 10:44:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DDevice_Impl.h"

#include <OEMath/OEMath.h>
#include <IOEConfigFileMgr.h>
#include <IOELogFileMgr.h>
#include <IOECore.h>
#include <IOEApp.h>
#include <IOERenderer.h>
#include <OEMsgID.h>
#include <IOEMsgMgr.h>

#include <OEOS.h>

// TODO: wrapped it
IDirect3D9* g_pD3D = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;
HWND g_hWnd = NULL;

static const tchar NODE_WINDOW_WIDTH[] = t("WindowWidth");
static const tchar NODE_WINDOW_HEIGHT[] = t("WindowHeight");
static const tchar NODE_CLASS_NAME[] = t("ClassName");
static const tchar NODE_WINDOW_NAME[] = t("WindowName");
static const tchar NODE_MAX_FPS[] = t("MaxFPS");

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const tchar CLASS_NAME[] = t("OriginEngine");
static const tchar WINDOW_NAME[] = t("Origin Engine");
static const float MAX_FPS = 60.0f;

static const tstring PARAM_HWIND = t("HWIND");

COED3DDevice_Impl::COED3DDevice_Impl()
{
	g_pOEDevice = this;
	Init();
	m_bOK = true;
}

COED3DDevice_Impl::~COED3DDevice_Impl()
{
	Destroy();
	g_pOEDevice = NULL;
}

void COED3DDevice_Impl::Init()
{
	// read from config file
	g_pOEConfigFileMgr->GetValue(m_nWindowWidth, NODE_WINDOW_WIDTH, WINDOW_WIDTH);
	g_pOEConfigFileMgr->GetValue(m_nWindowHeight, NODE_WINDOW_HEIGHT, WINDOW_HEIGHT);
	g_pOEConfigFileMgr->GetValue(m_strClassName, NODE_CLASS_NAME, CLASS_NAME);
	g_pOEConfigFileMgr->GetValue(m_strWindowName, NODE_WINDOW_NAME, WINDOW_NAME);
	g_pOEConfigFileMgr->GetValue(m_fMaxFPS, NODE_MAX_FPS, MAX_FPS);

	m_fPrevTime = 0.0f;
	m_fCurrTime = 0.0f;
	m_vD3DVertDecl.clear();

	m_fCurrFPS = 0.0f;
	m_fLastFPSTime = 0.0f;
	m_nFPSCount = 0;
	m_pFontFPS = NULL;
	m_pStringFPS = NULL;
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
	SAFE_RELEASE(m_pStringFPS);
	SAFE_RELEASE(m_pFontFPS);

	if (g_pd3dDevice) InternalDestroyD3D();
	if (g_hWnd) InternalDestroyWindow();
}

void COED3DDevice_Impl::StartPerform()
{
	// show window
	ShowWindow(g_hWnd, SW_NORMAL);
	UpdateWindow(g_hWnd);

	// logout start perform
	LOGOUT(t("StartPerforming ..."));

	// Create Render signal event
	HANDLE hTickEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	ResetEvent(hTickEvent);

	// Create time event
	UINT nDelay = (UINT)(1000.0f/m_fMaxFPS);
	if (nDelay < 1) nDelay = 1;
	MMRESULT hEventTimer = timeSetEvent(nDelay, 1, (LPTIMECALLBACK)hTickEvent, 0, TIME_PERIODIC|TIME_CALLBACK_EVENT_SET);

	// reset time
	m_fPrevTime = (float)timeGetTime()/1000.0f;
	m_fCurrTime = m_fPrevTime;

	// reset fps
	ResetFPS(m_fCurrTime);

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if(WAIT_OBJECT_0 == MsgWaitForMultipleObjects(1, &hTickEvent, FALSE, 1000, QS_ALLINPUT))
		{
			g_pOECore->Update();

			// calculate time
			m_fCurrTime = (float)timeGetTime()/1000.0f;
			PerformOnce(m_fCurrTime - m_fPrevTime);
			m_fPrevTime = m_fCurrTime;

			// calculate fps
			CalculateFPS();
		}
		else
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{
				if(msg.message == WM_QUIT) break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	if(hEventTimer)
	{
		timeKillEvent(hEventTimer);
		hEventTimer = NULL;
	}

	if(hTickEvent)
	{
		CloseHandle(hTickEvent);
		hTickEvent = NULL;
	}
}

void COED3DDevice_Impl::EndPerform()
{
	PostQuitMessage(0);
}

float COED3DDevice_Impl::GetFPS()
{
	return m_fCurrFPS;
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
		LOGOUT(t("IOEDevice::CreateVertDecl Failed"));
		return NULL;
	}

	m_vD3DVertDecl.push_back(pDecl);

	return pDecl;
}

bool COED3DDevice_Impl::GetDeviceParam(void* pData, const tstring& strParamName)
{
	if (strParamName == PARAM_HWIND)
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
		m_strClassName.c_str(),				// Pointer to a null-terminated string or is an atom
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
	g_hWnd = CreateWindow(m_strClassName.c_str(),
		m_strWindowName.c_str(),
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
		LOGOUT(t("COED3DDevice_Impl::InternalCreateWindow Failed"));
		return false;
	}

	LOGOUT(t("COED3DDevice_Impl::InternalCreateWindow OK"));
	return true;
}

void COED3DDevice_Impl::InternalDestroyWindow()
{
	if (g_hWnd)
	{
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		LOGOUT(t("COED3DDevice_Impl::InternalDestroyWindow OK"));
	}
}

bool COED3DDevice_Impl::InternalCreateD3D()
{
	if (g_pd3dDevice) InternalDestroyD3D();

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		LOGOUT(t("COED3DDevice_Impl::InternalCreateD3D Failed"));
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

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
		LOGOUT(t("COED3DDevice_Impl::InternalCreateD3D Failed"));
		InternalDestroyD3D();
		return false;
	}

	LOGOUT(t("COED3DDevice_Impl::InternalCreateD3D OK"));

	return true;
}

void COED3DDevice_Impl::InternalDestroyD3D()
{
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = NULL;
	}

	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;

		LOGOUT(t("COED3DDevice_Impl::InternalDestroyD3D OK"));
	}

	// TODO: check m_vD3DVertDecl whether is empty, and logout
}

void COED3DDevice_Impl::PerformOnce(float fDetailTime)
{
	g_pOEApp->Update(fDetailTime);

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// render scene
		g_pOERenderer->SetSampleFilter(IOERenderer::SF_LINEAR);
		g_pOERenderer->SetShader(NULL);
		g_pOERenderer->SetVertDecl(NULL);
		g_pOERenderer->SetTexture(NULL);
		g_pOEApp->Render(fDetailTime);

		// render fps
		RenderFPS();

		g_pd3dDevice->EndScene();
		g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}
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
}

void COED3DDevice_Impl::ResetFPS(float fLastTime)
{
	m_fLastFPSTime = fLastTime;
	m_nFPSCount = 0;
}

void COED3DDevice_Impl::CalculateFPS()
{
	++m_nFPSCount;
	float fDetailFPS = m_fCurrTime - m_fLastFPSTime;
	if (fDetailFPS > 1.0f)
	{
		m_fCurrFPS = m_nFPSCount/fDetailFPS;
		m_nFPSCount = 0;
		m_fLastFPSTime = m_fCurrTime;

		if (m_pStringFPS)
		{
			tstring strText;
			COEOS::strformat(strText, t("%.2f FPS"), m_fCurrFPS);
			m_pStringFPS->SetText(strText);
		}
	}
}

void COED3DDevice_Impl::RenderFPS()
{
	if (!m_pFontFPS)
	{
		SAFE_RELEASE(m_pStringFPS);
		SAFE_RELEASE(m_pFontFPS);

		m_pFontFPS = g_pOEUIFontMgr->CreateBitmapFont(t("12px_Tahoma.fnt"));
		m_pStringFPS = g_pOEUIStringMgr->CreateUIString(m_pFontFPS);
	}

	g_pOERenderer->SetSampleFilter(IOERenderer::SF_POINT);
	g_pOERenderer->SetFillMode(IOERenderer::FM_SOLID);
	g_pOERenderer->SetShader(NULL);
	g_pOERenderer->SetVertDecl(NULL);
	g_pOERenderer->SetTexture(NULL);
	if (m_pStringFPS) m_pStringFPS->Render(CPoint(0, 0));
	g_pOEUIRenderer->FlushAll();
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

			// send message
			COEMsg msg(OMI_LBUTTON_DOWN);
			msg.Write((int)x);
			msg.Write((int)y);
			g_pOEMsgMgr->SendMessageAndBlocked(&msg);
		}
		break;
	case WM_LBUTTONUP:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// send message
			COEMsg msg(OMI_LBUTTON_UP);
			msg.Write((int)x);
			msg.Write((int)y);
			g_pOEMsgMgr->SendMessageAndBlocked(&msg);

			ReleaseCapture();
		}
		break;
	case WM_MOUSEMOVE:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// send message
			COEMsg msg(OMI_MOUSE_MOVE);
			msg.Write((int)x-s_nLastMousePosX);
			msg.Write((int)y-s_nLastMousePosY);
			g_pOEMsgMgr->SendMessageAndBlocked(&msg);

			s_nLastMousePosX = x;
			s_nLastMousePosY = y;
		}
		break;
	case WM_KEYDOWN:
		{
			// send message
			COEMsg msg(OMI_KEY_DOWN);
			msg.Write((int)wParam);
			g_pOEMsgMgr->SendMessageAndBlocked(&msg);
		}
		break;
	case WM_KEYUP:
		{
			// send message
			COEMsg msg(OMI_KEY_UP);
			msg.Write((int)wParam);
			g_pOEMsgMgr->SendMessageAndBlocked(&msg);
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
