/*!
 * \file DlgWaveSetting.cpp
 * \date 28-11-2009 8:23:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "DlgWaveSetting.h"
#include <wx/xrc/xmlres.h>
#include <wx/clipbrd.h>

CDlgWaveSetting::CDlgWaveSetting()
{
	m_vVecFreq = CVector4(13.2f, 48.5f, 100.0f, 100.0f);
	m_vVecSpeed = CVector4(0.022f, 0.022f, 0.015f, 0.007f);
	m_vVecDir = CVector4(2.588f, 2.217f, 1.78f, 1.66f);
	m_vVecHeight = CVector4(11.0f, 3.0f, 2.25f, 1.5f);

	m_fTimeScale = 1.0f;
	m_fFreqScale = 1.0f;
	m_fSpeedScale = 1.0f;
	m_fHeightScale = 1.0f;

	InitCtrlInfo();
}

CDlgWaveSetting::~CDlgWaveSetting()
{
	// TODO: 
}

bool CDlgWaveSetting::Initialize()
{
	if (!wxXmlResource::Get()->LoadDialog(this, NULL, wxT("IDD_WAVESETTING"))) return false;

	for (int i = 0; i < CIT_NUM; ++i)
	{
		m_CtrlInfo[i].pSlider = (wxSlider*)this->FindWindow(m_CtrlInfo[i].strSldName);
		if (!m_CtrlInfo[i].pSlider) return false;

		m_CtrlInfo[i].pLabel = (wxStaticText*)this->FindWindow(m_CtrlInfo[i].strLblName);
		if (!m_CtrlInfo[i].pLabel) return false;

		m_CtrlInfo[i].pSlider->SetRange(m_CtrlInfo[i].nScrollRangeMin, m_CtrlInfo[i].nScrollRangeMax);
		float fDefValue = m_CtrlInfo[i].fDefValue*(m_CtrlInfo[i].nScrollRangeMax - m_CtrlInfo[i].nScrollRangeMin)/(m_CtrlInfo[i].fRangeMax - m_CtrlInfo[i].fRangeMin);
		m_CtrlInfo[i].pSlider->SetValue((int)fDefValue);

		SetValue(&m_CtrlInfo[i], m_CtrlInfo[i].fDefValue);

		this->Connect(wxXmlResource::GetXRCID(m_CtrlInfo[i].strSldName), wxEVT_SCROLL_CHANGED, (wxObjectEventFunction)(wxScrollEventFunction)&CDlgWaveSetting::OnScrollChanged);
		this->Connect(wxXmlResource::GetXRCID(m_CtrlInfo[i].strSldName), wxEVT_SCROLL_THUMBTRACK, (wxObjectEventFunction)(wxScrollEventFunction)&CDlgWaveSetting::OnScrollChanged);
	}

	this->Connect(wxXmlResource::GetXRCID(wxT("IDC_BTN_COPY")), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CDlgWaveSetting::OnBtnCopyClicked);

	return true;
}

CDlgWaveSetting::CONTROL_INFO* CDlgWaveSetting::GetCtrlInfoFromSliderID(int nID)
{
	for (int i = 0; i < CIT_NUM; ++i)
	{
		if (m_CtrlInfo[i].pSlider->GetId() == nID) return &m_CtrlInfo[i];
	}

	return NULL;
}

void CDlgWaveSetting::SetValue(CONTROL_INFO* pCtrlInfo, float fValue)
{
	if (fValue >= 10.0f)
	{
		wxString strValue = wxString::Format(wxT("%.1f"), fValue);
		pCtrlInfo->pLabel->SetLabel(strValue);
	}
	else
	{
		wxString strValue = wxString::Format(wxT("%.2f"), fValue);
		pCtrlInfo->pLabel->SetLabel(strValue);
	}

	switch (pCtrlInfo->eType)
	{
	case CIT_FREQ1:
		m_vVecFreq.x = fValue;
		break;
	case CIT_FREQ2:
		m_vVecFreq.y = fValue;
		break;
	case CIT_FREQ3:
		m_vVecFreq.z = fValue;
		break;
	case CIT_FREQ4:
		m_vVecFreq.w = fValue;
		break;
	case CIT_SPEED1:
		m_vVecSpeed.x = fValue;
		break;
	case CIT_SPEED2:
		m_vVecSpeed.y = fValue;
		break;
	case CIT_SPEED3:
		m_vVecSpeed.z = fValue;
		break;
	case CIT_SPEED4:
		m_vVecSpeed.w = fValue;
		break;
	case CIT_DIR1:
		m_vVecDir.x = fValue;
		m_vVecDirX.x = cosf(fValue);
		m_vVecDirY.x = sinf(fValue);
		break;
	case CIT_DIR2:
		m_vVecDir.y = fValue;
		m_vVecDirX.y = cosf(fValue);
		m_vVecDirY.y = sinf(fValue);
		break;
	case CIT_DIR3:
		m_vVecDir.z = fValue;
		m_vVecDirX.z = cosf(fValue);
		m_vVecDirY.z = sinf(fValue);
		break;
	case CIT_DIR4:
		m_vVecDir.w = fValue;
		m_vVecDirX.w = cosf(fValue);
		m_vVecDirY.w = sinf(fValue);
		break;
	case CIT_HEIGHT1:
		m_vVecHeight.x = fValue;
		break;
	case CIT_HEIGHT2:
		m_vVecHeight.y = fValue;
		break;
	case CIT_HEIGHT3:
		m_vVecHeight.z = fValue;
		break;
	case CIT_HEIGHT4:
		m_vVecHeight.w = fValue;
		break;
	case CIT_TIME_SCALE:
		m_fTimeScale = fValue;
		break;
	case CIT_FREQ_SCALE:
		m_fFreqScale = fValue;
		break;
	case CIT_SPEED_SCALE:
		m_fSpeedScale = fValue;
		break;
	case CIT_HEIGHT_SCALE:
		m_fHeightScale = fValue;
		break;
	}
}

void CDlgWaveSetting::OnScrollChanged(wxScrollEvent& event)
{
	CONTROL_INFO* pCtrlInfo = GetCtrlInfoFromSliderID(event.GetId());
	if (!pCtrlInfo) return;

	int nPos = event.GetPosition();
	float fValue = nPos*(pCtrlInfo->fRangeMax - pCtrlInfo->fRangeMin)/(pCtrlInfo->nScrollRangeMax - pCtrlInfo->nScrollRangeMin);
	SetValue(pCtrlInfo, fValue);
}

void CDlgWaveSetting::OnBtnCopyClicked(wxCommandEvent& event)
{
	wxClipboardLocker locker;
	if (!locker) return;

	if (!wxTheClipboard->Open()) return;

	wxString strTextData;
	strTextData += wxString::Format(wxT("\tm_vVecFreq = CVector4(%ff, %ff, %ff, %ff);\r\n"), m_vVecFreq.x, m_vVecFreq.y, m_vVecFreq.z, m_vVecFreq.w);
	strTextData += wxString::Format(wxT("\tm_vVecSpeed = CVector4(%ff, %ff, %ff, %ff);\r\n"), m_vVecSpeed.x, m_vVecSpeed.y, m_vVecSpeed.z, m_vVecSpeed.w);
	strTextData += wxString::Format(wxT("\tm_vVecDir = CVector4(%ff, %ff, %ff, %ff);\r\n"), m_vVecDir.x, m_vVecDir.y, m_vVecDir.z, m_vVecDir.w);
	strTextData += wxString::Format(wxT("\tm_vVecHeight = CVector4(%ff, %ff, %ff, %ff);\r\n"), m_vVecHeight.x, m_vVecHeight.y, m_vVecHeight.z, m_vVecHeight.w);
	strTextData += wxString::Format(wxT("\tm_fTimeScale = %ff;\r\n"), m_fTimeScale);
	strTextData += wxString::Format(wxT("\tm_fFreqScale = %ff;\r\n"), m_fFreqScale);
	strTextData += wxString::Format(wxT("\tm_fSpeedScale = %ff;\r\n"), m_fSpeedScale);
	strTextData += wxString::Format(wxT("\tm_fHeightScale = %ff;\r\n"), m_fHeightScale);

	wxTheClipboard->SetData(new wxTextDataObject(strTextData));
	wxTheClipboard->Close();
}

void CDlgWaveSetting::InitCtrlInfo()
{
	InitCtrlInfoItem(CIT_FREQ1, wxT("IDC_SLD_FREQUENCY1"), wxT("IDC_LBL_FREQUENCY1"), 0, 100, 0.0f, 100.0f, m_vVecFreq.x);
	InitCtrlInfoItem(CIT_FREQ2, wxT("IDC_SLD_FREQUENCY2"), wxT("IDC_LBL_FREQUENCY2"), 0, 100, 0.0f, 100.0f, m_vVecFreq.y);
	InitCtrlInfoItem(CIT_FREQ3, wxT("IDC_SLD_FREQUENCY3"), wxT("IDC_LBL_FREQUENCY3"), 0, 100, 0.0f, 100.0f, m_vVecFreq.z);
	InitCtrlInfoItem(CIT_FREQ4, wxT("IDC_SLD_FREQUENCY4"), wxT("IDC_LBL_FREQUENCY4"), 0, 100, 0.0f, 100.0f, m_vVecFreq.w);

	InitCtrlInfoItem(CIT_SPEED1, wxT("IDC_SLD_SPEED1"), wxT("IDC_LBL_SPEED1"), 0, 100, 0.0f, 1.0f, m_vVecSpeed.x);
	InitCtrlInfoItem(CIT_SPEED2, wxT("IDC_SLD_SPEED2"), wxT("IDC_LBL_SPEED2"), 0, 100, 0.0f, 1.0f, m_vVecSpeed.y);
	InitCtrlInfoItem(CIT_SPEED3, wxT("IDC_SLD_SPEED3"), wxT("IDC_LBL_SPEED3"), 0, 100, 0.0f, 1.0f, m_vVecSpeed.z);
	InitCtrlInfoItem(CIT_SPEED4, wxT("IDC_SLD_SPEED4"), wxT("IDC_LBL_SPEED4"), 0, 100, 0.0f, 1.0f, m_vVecSpeed.w);

	InitCtrlInfoItem(CIT_DIR1, wxT("IDC_SLD_DIRECTION1"), wxT("IDC_LBL_DIRECTION1"), 0, 100, 0.0f, OEMATH_2PI, m_vVecDir.x);
	InitCtrlInfoItem(CIT_DIR2, wxT("IDC_SLD_DIRECTION2"), wxT("IDC_LBL_DIRECTION2"), 0, 100, 0.0f, OEMATH_2PI, m_vVecDir.y);
	InitCtrlInfoItem(CIT_DIR3, wxT("IDC_SLD_DIRECTION3"), wxT("IDC_LBL_DIRECTION3"), 0, 100, 0.0f, OEMATH_2PI, m_vVecDir.z);
	InitCtrlInfoItem(CIT_DIR4, wxT("IDC_SLD_DIRECTION4"), wxT("IDC_LBL_DIRECTION4"), 0, 100, 0.0f, OEMATH_2PI, m_vVecDir.w);

	InitCtrlInfoItem(CIT_HEIGHT1, wxT("IDC_SLD_HEIGHT1"), wxT("IDC_LBL_HEIGHT1"), 0, 100, 0.0f, 100.0f, m_vVecHeight.x);
	InitCtrlInfoItem(CIT_HEIGHT2, wxT("IDC_SLD_HEIGHT2"), wxT("IDC_LBL_HEIGHT2"), 0, 100, 0.0f, 100.0f, m_vVecHeight.y);
	InitCtrlInfoItem(CIT_HEIGHT3, wxT("IDC_SLD_HEIGHT3"), wxT("IDC_LBL_HEIGHT3"), 0, 100, 0.0f, 100.0f, m_vVecHeight.z);
	InitCtrlInfoItem(CIT_HEIGHT4, wxT("IDC_SLD_HEIGHT4"), wxT("IDC_LBL_HEIGHT4"), 0, 100, 0.0f, 100.0f, m_vVecHeight.w);

	InitCtrlInfoItem(CIT_TIME_SCALE, wxT("IDC_SLD_TIMESCALE"), wxT("IDC_LBL_TIMESCALE"), 0, 100, 0.0f, 1.0f, m_fTimeScale);
	InitCtrlInfoItem(CIT_FREQ_SCALE, wxT("IDC_SLD_FREQUENCYSCALE"), wxT("IDC_LBL_FREQUENCYSCALE"), 0, 100, 0.0f, 10.0f, m_fFreqScale);
	InitCtrlInfoItem(CIT_SPEED_SCALE, wxT("IDC_SLD_SPEEDSCALE"), wxT("IDC_LBL_SPEEDSCALE"), 0, 100, 0.0f, 1.0f, m_fSpeedScale);
	InitCtrlInfoItem(CIT_HEIGHT_SCALE, wxT("IDC_SLD_HEIGHTSCALE"), wxT("IDC_LBL_HEIGHTSCALE"), 0, 100, 0.0f, 1.0f, m_fHeightScale);
}

void CDlgWaveSetting::InitCtrlInfoItem(CONTROL_ITEM_TYPE eType, const wxChar* pstrSldName, const wxChar* pstrLblName, int nScrollRangeMin, int nScrollRangeMax, float fRangeMin, float fRangeMax, float fDefValue)
{
	m_CtrlInfo[eType].eType = eType;
	m_CtrlInfo[eType].pSlider = NULL;
	m_CtrlInfo[eType].pLabel = NULL;
	m_CtrlInfo[eType].strSldName = pstrSldName;
	m_CtrlInfo[eType].strLblName = pstrLblName;
	m_CtrlInfo[eType].nScrollRangeMin = nScrollRangeMin;
	m_CtrlInfo[eType].nScrollRangeMax = nScrollRangeMax;
	m_CtrlInfo[eType].fRangeMin = fRangeMin;
	m_CtrlInfo[eType].fRangeMax = fRangeMax;
	m_CtrlInfo[eType].fDefValue = fDefValue;
}
