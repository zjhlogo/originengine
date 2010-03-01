/*!
 * \file MatrixElement.h
 * \date 27-2-2010 10:58:26
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MATRIXELEMENT_H__
#define __MATRIXELEMENT_H__

#include <wx/textctrl.h>

class CMatrixElement
{
public:
	CMatrixElement();
	~CMatrixElement();

	float GetK();
	void SetK(float k);

	const wxString& GetVar();
	void SetVar(const wxString& strVar);

	wxTextCtrl* GetTextCtrl();
	void SetTextCtrl(wxTextCtrl* pTextCtrl);

	const wxString& GetValue();
	void SetValue(const wxString& strValue);

private:
	float m_k;
	wxString m_strVar;
	wxTextCtrl* m_pTextCtrl;
	wxString m_strValue;

};
#endif // __MATRIXELEMENT_H__
