/*!
 * \file DlgWaveParam.h
 * \date 20-6-2009 22:40:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __DLGWAVEPARAM_H__
#define __DLGWAVEPARAM_H__

#include <windows.h>
#include <OEMath/OEMath.h>

class CDlgWaveParam
{
public:
	enum CONST_DEFINE
	{
		NUM_WAVE = 4,
		SLIDER_RANGE_MIN = 0,
		SLIDER_RANGE_MAX = 1000,

		SCALE_RANGE_MIN = 0,
		SCALE_RANGE_MAX = 20,
		SCALE_RANGE_MID = SCALE_RANGE_MAX/2,
	};

	typedef struct RANGE_tag
	{
		float fMin;
		float fMax;
	} RANGE;

public:
	CDlgWaveParam();
	~CDlgWaveParam();

	bool Initialize();

	void Show(bool bShow);
	bool IsVisible() const;

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
	void Init();
	void Destroy();

	static BOOL CALLBACK DialogProc(HWND hDlg, UINT nMsg, WPARAM wParam, LPARAM lParam);

	BOOL OnInitDialog(HWND hDlg);
	BOOL OnSlider(WPARAM wParam, LPARAM lParam);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	void SetSliderPos(HWND hDlg, int nSliderID, float fValue);
	void SetSliderPos(HWND hDlg, int nSliderID, int nPos);
	void SetScaleSliderPos(HWND hDlg, int nSliderID, int nPos, bool bSetPos);

	void UpdateValue(HWND hDlg, int nSliderID, float fValue);
	void CopyToClipboard();

private:
	HWND m_hWndDlg;

	RANGE m_RangeFreq;
	RANGE m_RangeSpeed;
	RANGE m_RangeDir;
	RANGE m_RangeHeight;

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

#endif // __DLGWAVEPARAM_H__
