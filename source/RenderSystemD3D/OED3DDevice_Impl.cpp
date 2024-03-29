/*!
 * \file OED3DDevice_Impl.cpp
 * \date 24-5-2009 10:44:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DDevice_Impl.h"

#include <libOEMath/OEMath.h>
#include <OECore/IOEConfigFileMgr.h>
#include <OEBase/IOELogFileMgr.h>
#include <OECore/IOERenderSystem.h>
#include <libOEMsg/OEMsgList.h>
#include <libOEMsg/OEMsgMouse.h>
#include <libOEMsg/OEMsgKeyboard.h>
#include <libOEMsg/OEMsgCommand.h>
#include <OEBase/IOEMsgMgr.h>
#include <assert.h>

#include <libOEBase/OEOS.h>

// TODO: wrapped it
IDirect3D9* g_pD3D = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;
HWND g_hWnd = NULL;

static const tchar NODE_WINDOW_WIDTH[] = TS("WindowWidth");
static const tchar NODE_WINDOW_HEIGHT[] = TS("WindowHeight");
static const tchar NODE_CLASS_NAME[] = TS("ClassName");
static const tchar NODE_WINDOW_NAME[] = TS("WindowName");
static const tchar NODE_MAX_FPS[] = TS("MaxFPS");

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const tchar CLASS_NAME[] = TS("OriginEngine");
static const tchar WINDOW_NAME[] = TS("Origin Engine");
static const float MAX_FPS = 60.0f;

static const tstring PARAM_HWIND = TS("HWIND");
static const tstring PARAM_WINDOW_WIDTH = TS("WINDOW_WIDTH");
static const tstring PARAM_WINDOW_HEIGHT = TS("WINDOW_HEIGHT");

COED3DDevice_Impl::COED3DDevice_Impl()
{
	assert(!g_pOEDevice);
	g_pOEDevice = this;
	m_bOK = Init();
}

COED3DDevice_Impl::~COED3DDevice_Impl()
{
	Destroy();
	g_pOEDevice = NULL;
}

bool COED3DDevice_Impl::Init()
{
	m_nWindowWidth = WINDOW_WIDTH;
	m_nWindowHeight = WINDOW_HEIGHT;
	m_strClassName = CLASS_NAME;
	m_strWindowName = WINDOW_NAME;
	m_fMaxFPS = MAX_FPS;

	m_fPrevTime = 0.0f;
	m_fCurrTime = 0.0f;
	m_fDetailTime = 0.0f;
	m_vD3DVertDecl.clear();

	return true;
}

void COED3DDevice_Impl::Destroy()
{
	// TODO: 
}

bool COED3DDevice_Impl::Initialize()
{
	// read from config file
	g_pOEConfigFileMgr->GetValue(m_nWindowWidth, NODE_WINDOW_WIDTH, WINDOW_WIDTH);
	g_pOEConfigFileMgr->GetValue(m_nWindowHeight, NODE_WINDOW_HEIGHT, WINDOW_HEIGHT);
	g_pOEConfigFileMgr->GetValue(m_strClassName, NODE_CLASS_NAME, CLASS_NAME);
	g_pOEConfigFileMgr->GetValue(m_strWindowName, NODE_WINDOW_NAME, WINDOW_NAME);
	g_pOEConfigFileMgr->GetValue(m_fMaxFPS, NODE_MAX_FPS, MAX_FPS);

	// create window
	if (!InternalCreateWindow())
	{
		Terminate();
		return false;
	}

	// create direct3d
	if (!InternalCreateD3D())
	{
		Terminate();
		return false;
	}

	// initialize
	InitializeD3D();

	return true;
}

void COED3DDevice_Impl::Terminate()
{
	if (g_pd3dDevice) InternalDestroyD3D();
	if (g_hWnd) InternalDestroyWindow();

	g_pd3dDevice = NULL;
	g_pD3D = NULL;
	g_hWnd = NULL;
}

void COED3DDevice_Impl::StartPerform()
{
	// show window
	ShowWindow(g_hWnd, SW_NORMAL);
	UpdateWindow(g_hWnd);

	// logout start perform
	LOGOUT(TS("StartPerforming ..."));

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
	m_fDetailTime = 0.0f;

	// notify start perform
	COEMsgCommand msgStartPerform(OMI_START_PERFORM);
	CallEvent(msgStartPerform);

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if(WAIT_OBJECT_0 == MsgWaitForMultipleObjects(1, &hTickEvent, FALSE, 1000, QS_ALLINPUT))
		{
			// calculate time
			m_fCurrTime = (float)timeGetTime()/1000.0f;
			m_fDetailTime = m_fCurrTime - m_fPrevTime;
			PerformOnce(m_fDetailTime);
			m_fPrevTime = m_fCurrTime;
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

float COED3DDevice_Impl::GetCurrTime() const
{
	return m_fCurrTime;
}

float COED3DDevice_Impl::GetDetailTime() const
{
	return m_fDetailTime;
}

IOEVertDecl* COED3DDevice_Impl::CreateVertDecl(const VERT_DECL_ELEMENT* pElement)
{
	for (TV_OED3D_VERTDECL::iterator it = m_vD3DVertDecl.begin(); it != m_vD3DVertDecl.end(); ++it)
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
		LOGOUT(TS("IOEDevice::CreateVertDecl Failed"));
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
	}
	else if (strParamName == PARAM_WINDOW_WIDTH)
	{
		*(int*)pData = m_nWindowWidth;
	}
	else if (strParamName == PARAM_WINDOW_HEIGHT)
	{
		*(int*)pData = m_nWindowHeight;
	}
	else
	{
		return false;
	}

	return true;
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
		LOGOUT(TS("COED3DDevice_Impl::InternalCreateWindow Failed"));
		return false;
	}

	LOGOUT(TS("COED3DDevice_Impl::InternalCreateWindow OK"));
	return true;
}

void COED3DDevice_Impl::InternalDestroyWindow()
{
	if (g_hWnd)
	{
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		LOGOUT(TS("COED3DDevice_Impl::InternalDestroyWindow OK"));
	}
}

bool COED3DDevice_Impl::InternalCreateD3D()
{
	if (g_pd3dDevice) InternalDestroyD3D();

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		LOGOUT(TS("COED3DDevice_Impl::InternalCreateD3D Failed"));
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
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
		LOGOUT(TS("COED3DDevice_Impl::InternalCreateD3D Failed"));
		InternalDestroyD3D();
		return false;
	}

	LOGOUT(TS("COED3DDevice_Impl::InternalCreateD3D OK"));

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

		LOGOUT(TS("COED3DDevice_Impl::InternalDestroyD3D OK"));
	}

	// TODO: check m_vD3DVertDecl whether is empty, and logout
}

void COED3DDevice_Impl::PerformOnce(float fDetailTime)
{
	// notify pre update
	COEMsgCommand msgPreUpdate(OMI_PRE_UPDATE);
	CallEvent(msgPreUpdate);

	// notify update
	COEMsgCommand msgUpdate(OMI_UPDATE);
	CallEvent(msgUpdate);

	// notify post update
	COEMsgCommand msgPostUpdate(OMI_POST_UPDATE);
	CallEvent(msgPostUpdate);

	// notify UI pre update
	COEMsgCommand msgUIPreUpdate(OMI_UI_PRE_UPDATE);
	CallEvent(msgUIPreUpdate);

	// notify UI update
	COEMsgCommand msgUIUpdate(OMI_UI_UPDATE);
	CallEvent(msgUIUpdate);

	// notify UI post update
	COEMsgCommand msgUIPostUpdate(OMI_UI_POST_UPDATE);
	CallEvent(msgUIPostUpdate);

	g_pOERenderSystem->ClearScreen(CSM_TARGET|CSM_ZBUFFER);
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// notify pre render 3d
		COEMsgCommand msgPreRender3D(OMI_PRE_RENDER);
		CallEvent(msgPreRender3D);

		// notify render 3d
		COEMsgCommand msgRender3D(OMI_RENDER);
		CallEvent(msgRender3D);

		// notify post render 3d
		COEMsgCommand msgPostRender3D(OMI_POST_RENDER);
		CallEvent(msgPostRender3D);

		g_pOERenderSystem->ClearScreen(CSM_ZBUFFER);
		// notify pre render 2d
		COEMsgCommand msgUIPreRender3D(OMI_UI_PRE_RENDER);
		CallEvent(msgUIPreRender3D);

		// notify render 2d
		COEMsgCommand msgUIRender3D(OMI_UI_RENDER);
		CallEvent(msgUIRender3D);

		// notify post render 2d
		COEMsgCommand msgUIPostRender3D(OMI_UI_POST_RENDER);
		CallEvent(msgUIPostRender3D);

		g_pd3dDevice->EndScene();
		g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}
}

void COED3DDevice_Impl::InitializeD3D()
{
	// initialize
	CMatrix4x4 matWorld;
	matWorld.Identity();
	g_pOERenderSystem->SetTransform(TT_WORLD, matWorld);

	CMatrix4x4 matView;
	COEMath::BuildLookAtMatrixLH(matView, COEMath::VECTOR_ZERO, COEMath::VECTOR_FORWARD, COEMath::VECTOR_UP);
	g_pOERenderSystem->SetTransform(TT_VIEW, matView);

	CMatrix4x4 matProj;
	COEMath::BuildProjectMatrixLH(matProj, COEMath::PI/4.0f, (float)m_nWindowWidth/(float)m_nWindowHeight, 1.0f, 10000.0f);
	g_pOERenderSystem->SetTransform(TT_PROJECTION, matProj);
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
			COEMsgMouse msg(OMI_LBUTTON_DOWN);
			msg.SetPos(x, y);
			g_pOEDevice->CallEvent(msg);
		}
		break;
	case WM_LBUTTONUP:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// send message
			COEMsgMouse msg(OMI_LBUTTON_UP);
			msg.SetPos(x, y);
			g_pOEDevice->CallEvent(msg);

			ReleaseCapture();
		}
		break;
	case WM_MOUSEMOVE:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// send message
			COEMsgMouse msg(OMI_MOUSE_MOVE);
			msg.SetPos((int)x-s_nLastMousePosX, (int)y-s_nLastMousePosY);
			g_pOEDevice->CallEvent(msg);

			s_nLastMousePosX = x;
			s_nLastMousePosY = y;
		}
		break;
	case WM_KEYDOWN:
		{
			// send message
			COEMsgKeyboard msg(OMI_KEY_DOWN);
			msg.SetKeyCode(wParam);
			g_pOEDevice->CallEvent(msg);
		}
		break;
	case WM_KEYUP:
		{
			// send message
			COEMsgKeyboard msg(OMI_KEY_UP);
			msg.SetKeyCode(wParam);
			g_pOEDevice->CallEvent(msg);
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
