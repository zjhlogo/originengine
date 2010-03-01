/*!
 * \file MatrixElement.cpp
 * \date 27-2-2010 10:58:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MatrixElement.h"

CMatrixElement::CMatrixElement()
{
	m_k = 1.0f;
	m_strVar.clear();
	m_pTextCtrl = NULL;
	m_strValue = wxT("1.0");
}

CMatrixElement::~CMatrixElement()
{
	// TODO: 
}

float CMatrixElement::GetK()
{
	return m_k;
}

void CMatrixElement::SetK(float k)
{
	m_k = k;
}

const wxString& CMatrixElement::GetVar()
{
	return m_strVar;
}

void CMatrixElement::SetVar(const wxString& strVar)
{
	m_strVar = strVar;
}

wxTextCtrl* CMatrixElement::GetTextCtrl()
{
	return m_pTextCtrl;
}

void CMatrixElement::SetTextCtrl(wxTextCtrl* pTextCtrl)
{
	m_pTextCtrl = pTextCtrl;
}

const wxString& CMatrixElement::GetValue()
{
	return m_strValue;
}

void CMatrixElement::SetValue(const wxString& strValue)
{
	// TODO: find out k
	// TODO: find out var
	// TODO: combine k and var into value
}
