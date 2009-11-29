/*!
 * \file DlgWaveSetting.h
 * \date 28-11-2009 8:22:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __DLGWAVESETTING_H__
#define __DLGWAVESETTING_H__

#include <OEMath/OEMath.h>

#include <wx/dialog.h>
#include <wx/slider.h>
#include <wx/stattext.h>

class CDlgWaveSetting : public wxDialog
{
public:
	enum CONTROL_ITEM_TYPE
	{
		CIT_FREQ1 = 0,
		CIT_FREQ2,
		CIT_FREQ3,
		CIT_FREQ4,

		CIT_SPEED1,
		CIT_SPEED2,
		CIT_SPEED3,
		CIT_SPEED4,

		CIT_DIR1,
		CIT_DIR2,
		CIT_DIR3,
		CIT_DIR4,

		CIT_HEIGHT1,
		CIT_HEIGHT2,
		CIT_HEIGHT3,
		CIT_HEIGHT4,

		CIT_TIME_SCALE,
		CIT_FREQ_SCALE,
		CIT_SPEED_SCALE,
		CIT_HEIGHT_SCALE,

		CIT_NUM,
	};

	typedef struct CONTROL_INFO_tag
	{
		CONTROL_ITEM_TYPE eType;
		wxSlider* pSlider;
		wxStaticText* pLabel;
		wxString strSldName;
		wxString strLblName;
		int nScrollRangeMin;
		int nScrollRangeMax;
		float fRangeMin;
		float fRangeMax;
		float fDefValue;
	} CONTROL_INFO;

public:
	CDlgWaveSetting();
	virtual ~CDlgWaveSetting();

	bool Initialize();

	const CVector4& GetVecFreq() {return m_vVecFreq;};
	const CVector4& GetVecSpeed() {return m_vVecSpeed;};
	const CVector4& GetVecDirX() {return m_vVecDirX;};
	const CVector4& GetVecDirY() {return m_vVecDirY;};
	const CVector4& GetVecHeight() {return m_vVecHeight;};

	float GetTimeScale() {return m_fTimeScale;};
	float GetFreqScale() {return m_fFreqScale;};
	float GetSpeedScale() {return m_fSpeedScale;};
	float GetHeightScale() {return m_fHeightScale;};

private:
	void InitCtrlInfo();
	void InitCtrlInfoItem(CONTROL_ITEM_TYPE eType, const wxChar* pstrSldName, const wxChar* pstrLblName, int nScrollRangeMin, int nScrollRangeMax, float fRangeMin, float fRangeMax, float fDefValue);

	CONTROL_INFO* GetCtrlInfoFromSliderID(int nID);
	void SetValue(CONTROL_INFO* pCtrlInfo, float fValue);

	void OnScrollChanged(wxScrollEvent& event);
	void OnBtnCopyClicked(wxCommandEvent& event);

private:
	CONTROL_INFO m_CtrlInfo[CIT_NUM];

	CVector4 m_vVecFreq;
	CVector4 m_vVecSpeed;
	CVector4 m_vVecDir;
	CVector4 m_vVecHeight;

	CVector4 m_vVecDirX;
	CVector4 m_vVecDirY;

	float m_fTimeScale;
	float m_fFreqScale;
	float m_fSpeedScale;
	float m_fHeightScale;

};

#endif // __DLGWAVESETTING_H__
