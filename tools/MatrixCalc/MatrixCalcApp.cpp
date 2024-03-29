/*!
 * \file MatrixCalcApp.cpp
 * \date 27-2-2010 9:09:52
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MatrixCalcApp.h"
#include "DlgMatrixCalc.h"
#include "resource.h"
#include "../common/wxCommonHelper.h"

#include <wx/frame.h>
#include <wx/xrc/xmlres.h>

IMPLEMENT_APP(CMatrixCalcApp);

CMatrixCalcApp::CMatrixCalcApp()
{
	// TODO: 
}

CMatrixCalcApp::~CMatrixCalcApp()
{
	// TODO: 
}

bool CMatrixCalcApp::OnInit()
{
	if (!wxCommonHelper::InitializeXrc()) return false;

	if (!wxCommonHelper::AddMemoryXrc(TS("XRC"), IDR_XRC_DLGMATRIXCALC, TS("DlgMatrixCalc.xrc")))
	{
		// TODO: logout
		return false;
	}

	CDlgMatrixCalc dlg;
	dlg.ShowModal();

	return false;
}
