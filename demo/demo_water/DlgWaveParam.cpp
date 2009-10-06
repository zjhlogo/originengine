/*!
 * \file DlgWaveParam.cpp
 * \date 20-6-2009 22:41:07
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "DlgWaveParam.h"
#include "resource.h"

#include <CommCtrl.h>
#include <OEInterfaces.h>
#include <OEOS.h>

CDlgWaveParam* g_pDlgWaveParam = NULL;

CDlgWaveParam::CDlgWaveParam()
{
	g_pDlgWaveParam = this;
	Init();
}

CDlgWaveParam::~CDlgWaveParam()
{
	Destroy();
	g_pDlgWaveParam = NULL;
}

void CDlgWaveParam::Init()
{
	m_hWndDlg = NULL;

	m_RangeFreq.fMin = 0.0f; m_RangeFreq.fMax = 100.0f;
	m_RangeSpeed.fMin = 0.0f; m_RangeSpeed.fMax = 1.0f;
	m_RangeDir.fMin = 0.0f; m_RangeDir.fMax = 2.0f*OEMATH_PI;
	m_RangeHeight.fMin = 0.0f; m_RangeHeight.fMax = 100.0f;

	m_vVecFreq = CVector4(13.2f, 48.5f, 100.0f, 100.0f);
	m_vVecSpeed = CVector4(0.022f, 0.022f, 0.015f, 0.007f);
	m_vVecDir = CVector4(2.588f, 2.217f, 1.78f, 1.66f);
	m_vVecHeight = CVector4(11.0f, 3.0f, 2.25f, 1.5f);

	m_fTimeScale = 1.0f;
	m_fFreqScale = 1.0f;
	m_fSpeedScale = 1.0f;
	m_fHeightScale = 1.0f;

	m_vVecDirX = CVector4(cosf(m_vVecDir.x), cosf(m_vVecDir.y), cosf(m_vVecDir.z), cosf(m_vVecDir.w));
	m_vVecDirY = CVector4(sinf(m_vVecDir.x), sinf(m_vVecDir.y), sinf(m_vVecDir.z), sinf(m_vVecDir.w));
}

void CDlgWaveParam::Destroy()
{
	if (m_hWndDlg)
	{
		DestroyWindow(m_hWndDlg);
		m_hWndDlg = NULL;
	}
}

bool CDlgWaveParam::Initialize()
{
	InitCommonControls();

	HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);
	m_hWndDlg = CreateDialogParam((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_WAVE_PARAMETER), NULL, CDlgWaveParam::DialogProc, NULL);
	if (!m_hWndDlg) return false;

	return true;
}

void CDlgWaveParam::Show(bool bShow)
{
	if (bShow) ShowWindow(m_hWndDlg, SW_SHOWNORMAL);
	else ShowWindow(m_hWndDlg, SW_HIDE);
}

BOOL CALLBACK CDlgWaveParam::DialogProc(HWND hDlg, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch(nMsg)
	{
	case WM_INITDIALOG:
        {
			return g_pDlgWaveParam->OnInitDialog(hDlg);
        }
		break;
	case WM_CLOSE:
        {
            EndDialog(hDlg, 0);
        }
		break;
	case WM_HSCROLL:
		{
			return g_pDlgWaveParam->OnSlider(wParam, lParam);
		}
		break;
	case WM_COMMAND:
		{
			return g_pDlgWaveParam->OnCommand(wParam, lParam);
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}

	return TRUE;
}

bool CDlgWaveParam::IsVisible() const
{
	if (IsWindowVisible(m_hWndDlg)) return true;

	return false;
}

BOOL CDlgWaveParam::OnInitDialog(HWND hDlg)
{
	for (int i = IDC_SLD_FREQUENCY1; i <= IDC_SLD_HEIGHT4; ++i)
	{
		SendDlgItemMessage(hDlg, i, TBM_SETRANGE, TRUE, MAKELONG(SLIDER_RANGE_MIN, SLIDER_RANGE_MAX));
	}

	for (int i = IDC_SLD_TIMESCALE; i <= IDC_SLD_HEIGHTSCALE; ++i)
	{
		SendDlgItemMessage(hDlg, i, TBM_SETRANGE, TRUE, MAKELONG(SCALE_RANGE_MIN, SCALE_RANGE_MAX));
		SetScaleSliderPos(hDlg, i, SCALE_RANGE_MID, true);
	}

	SetSliderPos(hDlg, IDC_SLD_FREQUENCY1, m_vVecFreq.x);
	SetSliderPos(hDlg, IDC_SLD_FREQUENCY2, m_vVecFreq.y);
	SetSliderPos(hDlg, IDC_SLD_FREQUENCY3, m_vVecFreq.z);
	SetSliderPos(hDlg, IDC_SLD_FREQUENCY4, m_vVecFreq.w);

	SetSliderPos(hDlg, IDC_SLD_SPEED1, m_vVecSpeed.x);
	SetSliderPos(hDlg, IDC_SLD_SPEED2, m_vVecSpeed.y);
	SetSliderPos(hDlg, IDC_SLD_SPEED3, m_vVecSpeed.z);
	SetSliderPos(hDlg, IDC_SLD_SPEED4, m_vVecSpeed.w);

	SetSliderPos(hDlg, IDC_SLD_DIRECTION1, m_vVecDir.x);
	SetSliderPos(hDlg, IDC_SLD_DIRECTION2, m_vVecDir.y);
	SetSliderPos(hDlg, IDC_SLD_DIRECTION3, m_vVecDir.z);
	SetSliderPos(hDlg, IDC_SLD_DIRECTION4, m_vVecDir.w);

	SetSliderPos(hDlg, IDC_SLD_HEIGHT1, m_vVecHeight.x);
	SetSliderPos(hDlg, IDC_SLD_HEIGHT2, m_vVecHeight.y);
	SetSliderPos(hDlg, IDC_SLD_HEIGHT3, m_vVecHeight.z);
	SetSliderPos(hDlg, IDC_SLD_HEIGHT4, m_vVecHeight.w);

	return TRUE;
}

BOOL CDlgWaveParam::OnSlider(WPARAM wParam, LPARAM lParam)
{
	int nCtrlID = 0;
	int nPos = 0;

	switch (LOWORD(wParam))
	{
	case TB_THUMBPOSITION:
	case TB_THUMBTRACK:
		{
			nPos = (short)HIWORD(wParam);
			nCtrlID = GetDlgCtrlID((HWND)lParam);
		}
		break;
	default:
		{
			nPos = (int)SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
			nCtrlID = GetDlgCtrlID((HWND)lParam);
		}
		break;
	}

	switch (nCtrlID)
	{
	case IDC_SLD_TIMESCALE:
	case IDC_SLD_FREQSCALE:
	case IDC_SLD_SPEEDSCALE:
	case IDC_SLD_HEIGHTSCALE:
		{
			SetScaleSliderPos(m_hWndDlg, nCtrlID, nPos, false);
		}
		break;
	case IDC_SLD_FREQUENCY1:
	case IDC_SLD_FREQUENCY2:
	case IDC_SLD_FREQUENCY3:
	case IDC_SLD_FREQUENCY4:
	case IDC_SLD_SPEED1:
	case IDC_SLD_SPEED2:
	case IDC_SLD_SPEED3:
	case IDC_SLD_SPEED4:
	case IDC_SLD_DIRECTION1:
	case IDC_SLD_DIRECTION2:
	case IDC_SLD_DIRECTION3:
	case IDC_SLD_DIRECTION4:
	case IDC_SLD_HEIGHT1:
	case IDC_SLD_HEIGHT2:
	case IDC_SLD_HEIGHT3:
	case IDC_SLD_HEIGHT4:
		{
			SetSliderPos(m_hWndDlg, nCtrlID, nPos);
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}

	return TRUE;
}

BOOL CDlgWaveParam::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BTN_COPY:
		{
			CopyToClipboard();
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}

	return TRUE;
}

void CDlgWaveParam::SetSliderPos(HWND hDlg, int nSliderID, float fValue)
{
	int nPos = 0;

	switch (nSliderID)
	{
	case IDC_SLD_FREQUENCY1:
	case IDC_SLD_FREQUENCY2:
	case IDC_SLD_FREQUENCY3:
	case IDC_SLD_FREQUENCY4:
		{
			nPos = SLIDER_RANGE_MIN + (int)(fValue*(SLIDER_RANGE_MAX-SLIDER_RANGE_MIN)/(m_RangeFreq.fMax-m_RangeFreq.fMin));
		}
		break;
	case IDC_SLD_SPEED1:
	case IDC_SLD_SPEED2:
	case IDC_SLD_SPEED3:
	case IDC_SLD_SPEED4:
		{
			nPos = SLIDER_RANGE_MIN + (int)(fValue*(SLIDER_RANGE_MAX-SLIDER_RANGE_MIN)/(m_RangeSpeed.fMax-m_RangeSpeed.fMin));
		}
		break;
	case IDC_SLD_DIRECTION1:
	case IDC_SLD_DIRECTION2:
	case IDC_SLD_DIRECTION3:
	case IDC_SLD_DIRECTION4:
		{
			nPos = SLIDER_RANGE_MIN + (int)(fValue*(SLIDER_RANGE_MAX-SLIDER_RANGE_MIN)/(m_RangeDir.fMax-m_RangeDir.fMin));
		}
		break;
	case IDC_SLD_HEIGHT1:
	case IDC_SLD_HEIGHT2:
	case IDC_SLD_HEIGHT3:
	case IDC_SLD_HEIGHT4:
		{
			nPos = SLIDER_RANGE_MIN + (int)(fValue*(SLIDER_RANGE_MAX-SLIDER_RANGE_MIN)/(m_RangeHeight.fMax-m_RangeHeight.fMin));
		}
		break;
	case IDC_SLD_TIMESCALE:
	case IDC_SLD_FREQSCALE:
	case IDC_SLD_SPEEDSCALE:
	case IDC_SLD_HEIGHTSCALE:
		{
			// TODO: 
			return;
		}
		break;
	default:
		{
			return;
		}
		break;
	}

	SendDlgItemMessage(hDlg, nSliderID, TBM_SETPOS, TRUE, nPos);

	UpdateValue(hDlg, nSliderID, fValue);
}

void CDlgWaveParam::SetSliderPos(HWND hDlg, int nSliderID, int nPos)
{
	float fValue = 0.0f;

	switch (nSliderID)
	{
	case IDC_SLD_FREQUENCY1:
	case IDC_SLD_FREQUENCY2:
	case IDC_SLD_FREQUENCY3:
	case IDC_SLD_FREQUENCY4:
		{
			fValue = m_RangeFreq.fMin + nPos*(m_RangeFreq.fMax-m_RangeFreq.fMin)/(float)(SLIDER_RANGE_MAX-SLIDER_RANGE_MIN);
		}
		break;
	case IDC_SLD_SPEED1:
	case IDC_SLD_SPEED2:
	case IDC_SLD_SPEED3:
	case IDC_SLD_SPEED4:
		{
			fValue = m_RangeSpeed.fMin + nPos*(m_RangeSpeed.fMax-m_RangeSpeed.fMin)/(float)(SLIDER_RANGE_MAX-SLIDER_RANGE_MIN);
		}
		break;
	case IDC_SLD_DIRECTION1:
	case IDC_SLD_DIRECTION2:
	case IDC_SLD_DIRECTION3:
	case IDC_SLD_DIRECTION4:
		{
			fValue = m_RangeDir.fMin + nPos*(m_RangeDir.fMax-m_RangeDir.fMin)/(float)(SLIDER_RANGE_MAX-SLIDER_RANGE_MIN);
		}
		break;
	case IDC_SLD_HEIGHT1:
	case IDC_SLD_HEIGHT2:
	case IDC_SLD_HEIGHT3:
	case IDC_SLD_HEIGHT4:
		{
			fValue = m_RangeHeight.fMin + nPos*(m_RangeHeight.fMax-m_RangeHeight.fMin)/(float)(SLIDER_RANGE_MAX-SLIDER_RANGE_MIN);
		}
		break;
	default:
		{
			return;
		}
		break;
	}

	UpdateValue(hDlg, nSliderID, fValue);
}

void CDlgWaveParam::SetScaleSliderPos(HWND hDlg, int nSliderID, int nPos, bool bSetPos)
{
	float fValue = 0.0f;

	switch (nSliderID)
	{
	case IDC_SLD_TIMESCALE:
	case IDC_SLD_FREQSCALE:
	case IDC_SLD_SPEEDSCALE:
	case IDC_SLD_HEIGHTSCALE:
		{
			int nOffset = nPos - SCALE_RANGE_MID;
			fValue = 1.0f;
			if (nOffset < 0) fValue /= (-nOffset+1);
			else if (nOffset > 0) fValue *= (nOffset+1);

			if (bSetPos) SendDlgItemMessage(hDlg, nSliderID, TBM_SETPOS, TRUE, nPos);
		}
		break;
	default:
		{
			return;
		}
		break;
	}

	UpdateValue(hDlg, nSliderID, fValue);
}

void CDlgWaveParam::UpdateValue(HWND hDlg, int nSliderID, float fValue)
{
	int nStaticID = 0;

	switch (nSliderID)
	{
	case IDC_SLD_FREQUENCY1:
		m_vVecFreq.x = fValue;
		nStaticID = IDC_STA_FREQUENCY1;
		break;
	case IDC_SLD_FREQUENCY2:
		m_vVecFreq.y = fValue;
		nStaticID = IDC_STA_FREQUENCY2;
		break;
	case IDC_SLD_FREQUENCY3:
		m_vVecFreq.z = fValue;
		nStaticID = IDC_STA_FREQUENCY3;
		break;
	case IDC_SLD_FREQUENCY4:
		m_vVecFreq.w = fValue;
		nStaticID = IDC_STA_FREQUENCY4;
		break;
	case IDC_SLD_SPEED1:
		m_vVecSpeed.x = fValue;
		nStaticID = IDC_STA_SPEED1;
		break;
	case IDC_SLD_SPEED2:
		m_vVecSpeed.y = fValue;
		nStaticID = IDC_STA_SPEED2;
		break;
	case IDC_SLD_SPEED3:
		m_vVecSpeed.z = fValue;
		nStaticID = IDC_STA_SPEED3;
		break;
	case IDC_SLD_SPEED4:
		m_vVecSpeed.w = fValue;
		nStaticID = IDC_STA_SPEED4;
		break;
	case IDC_SLD_DIRECTION1:
		m_vVecDir.x = fValue;
		m_vVecDirX.x = cosf(fValue);
		m_vVecDirY.x = sinf(fValue);
		nStaticID = IDC_STA_DIRECTION1;
		break;
	case IDC_SLD_DIRECTION2:
		m_vVecDir.y = fValue;
		m_vVecDirX.y = cosf(fValue);
		m_vVecDirY.y = sinf(fValue);
		nStaticID = IDC_STA_DIRECTION2;
		break;
	case IDC_SLD_DIRECTION3:
		m_vVecDir.z = fValue;
		m_vVecDirX.z = cosf(fValue);
		m_vVecDirY.z = sinf(fValue);
		nStaticID = IDC_STA_DIRECTION3;
		break;
	case IDC_SLD_DIRECTION4:
		m_vVecDir.w = fValue;
		m_vVecDirX.w = cosf(fValue);
		m_vVecDirY.w = sinf(fValue);
		nStaticID = IDC_STA_DIRECTION4;
		break;
	case IDC_SLD_HEIGHT1:
		m_vVecHeight.x = fValue;
		nStaticID = IDC_STA_HEIGHT1;
		break;
	case IDC_SLD_HEIGHT2:
		m_vVecHeight.y = fValue;
		nStaticID = IDC_STA_HEIGHT2;
		break;
	case IDC_SLD_HEIGHT3:
		m_vVecHeight.z = fValue;
		nStaticID = IDC_STA_HEIGHT3;
		break;
	case IDC_SLD_HEIGHT4:
		m_vVecHeight.w = fValue;
		nStaticID = IDC_STA_HEIGHT4;
		break;
	case IDC_SLD_TIMESCALE:
		m_fTimeScale = fValue;
		nStaticID = IDC_STA_TIMESCALE;
		break;
	case IDC_SLD_FREQSCALE:
		m_fFreqScale = fValue;
		nStaticID = IDC_STA_FREQSCALE;
		break;
	case IDC_SLD_SPEEDSCALE:
		m_fSpeedScale = fValue;
		nStaticID = IDC_STA_SPEEDSCALE;
		break;
	case IDC_SLD_HEIGHTSCALE:
		m_fHeightScale = fValue;
		nStaticID = IDC_STA_HEIGHTSCALE;
		break;
	}

	tstring strBuffer;
	COEOS::float2str(strBuffer, fValue);
	SetDlgItemText(hDlg, nStaticID, (LPTSTR)strBuffer.c_str());
}

void CDlgWaveParam::CopyToClipboard()
{
	tstring strData;
	tstring strBuffer;

	COEOS::strformat(strBuffer, t("\tm_vVecFreq = CVector4(%ff, %ff, %ff, %ff);\r\n"), m_vVecFreq.x, m_vVecFreq.y, m_vVecFreq.z, m_vVecFreq.w);
	strData += strBuffer;

	COEOS::strformat(strBuffer, t("\tm_vVecSpeed = CVector4(%ff, %ff, %ff, %ff);\r\n"), m_vVecSpeed.x, m_vVecSpeed.y, m_vVecSpeed.z, m_vVecSpeed.w);
	strData += strBuffer;

	COEOS::strformat(strBuffer, t("\tm_vVecDir = CVector4(%ff, %ff, %ff, %ff);\r\n"), m_vVecDir.x, m_vVecDir.y, m_vVecDir.z, m_vVecDir.w);
	strData += strBuffer;

	COEOS::strformat(strBuffer, t("\tm_vVecHeight = CVector4(%ff, %ff, %ff, %ff);\r\n"), m_vVecHeight.x, m_vVecHeight.y, m_vVecHeight.z, m_vVecHeight.w);
	strData += strBuffer;

	COEOS::strformat(strBuffer, t("\tm_fTimeScale = %ff;\r\n"), m_fTimeScale);
	strData += strBuffer;

	COEOS::strformat(strBuffer, t("\tm_fFreqScale = %ff;\r\n"), m_fFreqScale);
	strData += strBuffer;

	COEOS::strformat(strBuffer, t("\tm_fSpeedScale = %ff;\r\n"), m_fSpeedScale);
	strData += strBuffer;

	COEOS::strformat(strBuffer, t("\tm_fHeightScale = %ff;\r\n"), m_fHeightScale);
	strData += strBuffer;

	if (!OpenClipboard(m_hWndDlg)) return;
	EmptyClipboard();

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strData.length()*sizeof(tchar));
	void* pMem = GlobalLock(hMem);
	memcpy(pMem, strData.c_str(), strData.length()*sizeof(tchar));
	GlobalUnlock(hMem);

	SetClipboardData(CF_UNICODETEXT, hMem);

	CloseClipboard();
}
