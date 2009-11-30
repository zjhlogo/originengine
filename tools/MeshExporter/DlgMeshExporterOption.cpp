/*!
 * \file DlgMeshExporterOption.cpp
 * \date 11-30-2009 13:46:36
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "DlgMeshExporterOption.h"
#include <wx/xrc/xmlres.h>

CDlgMeshExporterOption::CDlgMeshExporterOption()
{
	// TODO: 
}

CDlgMeshExporterOption::~CDlgMeshExporterOption()
{
	// TODO: 
}

bool CDlgMeshExporterOption::Initialize(wxWindow* pParent)
{
	if (!wxXmlResource::Get()->LoadDialog(this, pParent, wxT("IDD_MESH_EXPORTER_OPTION"))) return false;

	return true;
}
