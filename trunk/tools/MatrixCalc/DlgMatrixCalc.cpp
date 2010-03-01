/*!
 * \file DlgMatrixCalc.cpp
 * \date 27-2-2010 9:16:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "DlgMatrixCalc.h"
#include <wx/xrc/xmlres.h>

BEGIN_EVENT_TABLE(CDlgMatrixCalc, wxDialog)
	EVT_BUTTON(XRCID("ID_BTN_CALCULATE"), CDlgMatrixCalc::OnBtnCalculateClick)
	EVT_BUTTON(XRCID("ID_BTN_CLOSE"), CDlgMatrixCalc::OnBtnCloseClick)
END_EVENT_TABLE()

CDlgMatrixCalc::CDlgMatrixCalc()
{
	wxXmlResource::Get()->LoadDialog(this, NULL, wxT("ID_DLG_MATRIXCALC"));

	for (int i = 0; i < MATRIX_ELEMENT_SIZE; ++i)
	{
		wxString strTextName = wxString::Format(wxT("ID_TEXTCTRL%d"), i);
		wxTextCtrl* pTextCtrl = (wxTextCtrl*)this->FindWindowByName(strTextName);
		wxASSERT(pTextCtrl);
		m_MatrixA[i].SetTextCtrl(pTextCtrl);
	}

	for (int i = 0; i < MATRIX_ELEMENT_SIZE; ++i)
	{
		wxString strTextName = wxString::Format(wxT("ID_TEXTCTRL%d"), i+MATRIX_ELEMENT_SIZE);
		wxTextCtrl* pTextCtrl = (wxTextCtrl*)this->FindWindowByName(strTextName);
		wxASSERT(pTextCtrl);
		m_MatrixB[i].SetTextCtrl(pTextCtrl);
	}

	for (int i = 0; i < MATRIX_ELEMENT_SIZE; ++i)
	{
		wxString strTextName = wxString::Format(wxT("ID_TEXTCTRL%d"), i+2*MATRIX_ELEMENT_SIZE);
		wxTextCtrl* pTextCtrl = (wxTextCtrl*)this->FindWindowByName(strTextName);
		wxASSERT(pTextCtrl);
		m_MatrixResult[i].SetTextCtrl(pTextCtrl);
	}

	ResetMatrixElement();
}

CDlgMatrixCalc::~CDlgMatrixCalc()
{
	// TODO: 
}

void CDlgMatrixCalc::OnBtnCalculateClick(wxCommandEvent& event)
{
	// TODO: 
}

void CDlgMatrixCalc::OnBtnCloseClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CDlgMatrixCalc::ResetMatrixElement()
{
	for (int i = 0; i < MATRIX_ROW; ++i)
	{
		for (int j = 0; j < MATRIX_COL; ++j)
		{
			int nIndex = i*MATRIX_COL+j;
			if (i == j)
			{
				m_MatrixA[nIndex].SetValue(wxT("1"));
			}
			else
			{
				m_MatrixA[nIndex].SetValue(wxT("0"));
			}
		}
	}

	for (int i = 0; i < MATRIX_ROW; ++i)
	{
		for (int j = 0; j < MATRIX_COL; ++j)
		{
			int nIndex = i*MATRIX_COL+j;
			if (i == j)
			{
				m_MatrixB[nIndex].SetValue(wxT("1"));
			}
			else
			{
				m_MatrixB[nIndex].SetValue(wxT("0"));
			}
		}
	}

	for (int i = 0; i < MATRIX_ROW; ++i)
	{
		for (int j = 0; j < MATRIX_COL; ++j)
		{
			int nIndex = i*MATRIX_COL+j;
			if (i == j)
			{
				m_MatrixResult[nIndex].SetValue(wxT("1"));
			}
			else
			{
				m_MatrixResult[nIndex].SetValue(wxT("0"));
			}
		}
	}
}
