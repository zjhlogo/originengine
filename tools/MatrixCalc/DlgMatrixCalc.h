/*!
 * \file DlgMatrixCalc.h
 * \date 27-2-2010 9:16:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __DLGMATRIXCALC_H__
#define __DLGMATRIXCALC_H__

#include <wx/dialog.h>
#include "MatrixElement.h"

class CDlgMatrixCalc : public wxDialog
{
	DECLARE_EVENT_TABLE()

public:
	enum CONST_DEFINE
	{
		MATRIX_ROW = 4,
		MATRIX_COL = 4,
		MATRIX_ELEMENT_SIZE = MATRIX_ROW*MATRIX_COL,
	};

public:
	CDlgMatrixCalc();
	virtual ~CDlgMatrixCalc();

private:
	void OnBtnCalculateClick(wxCommandEvent& event);
	void OnBtnCloseClick(wxCommandEvent& event);

	void ResetMatrixElement();
	void SetupMatrixElement();

private:
	CMatrixElement m_MatrixA[MATRIX_ELEMENT_SIZE];
	CMatrixElement m_MatrixB[MATRIX_ELEMENT_SIZE];
	CMatrixElement m_MatrixResult[MATRIX_ELEMENT_SIZE];

};
#endif // __DLGMATRIXCALC_H__
