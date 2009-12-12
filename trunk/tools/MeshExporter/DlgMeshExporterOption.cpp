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
	m_pChkExportMesh = NULL;
	m_pChkExportSkelecton = NULL;
	m_pChklMeshOption = NULL;
	m_pChklSkelectonOption = NULL;

	m_bExportMesh = true;
	m_bExportSkelecton = true;
	m_nVertexFlag = VF_POSITION|VF_TEXCOORD0|VF_NORMAL|VF_SKELECTON;
}

CDlgMeshExporterOption::~CDlgMeshExporterOption()
{
	// TODO: 
}

bool CDlgMeshExporterOption::Initialize(wxWindow* pParent)
{
	if (!wxXmlResource::Get()->LoadDialog(this, pParent, wxT("IDD_MESH_EXPORTER_OPTION"))) return false;

	m_pChkExportMesh = (wxCheckBox*)this->FindWindow(wxT("IDC_CHK_EXPORTMESH"));
	m_pChkExportSkelecton = (wxCheckBox*)this->FindWindow(wxT("IDC_CHK_EXPORTSKELECTON"));
	m_pChklMeshOption = (wxCheckListBox*)this->FindWindow(wxT("IDC_CHKL_MESHOPTION"));
	m_pChklSkelectonOption = (wxCheckListBox*)this->FindWindow(wxT("IDC_CHKL_SKELECTONOPTION"));

	this->Connect(wxXmlResource::GetXRCID(wxT("IDC_CHK_EXPORTMESH")), wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&CDlgMeshExporterOption::OnChkExportMeshClicked);
	this->Connect(wxXmlResource::GetXRCID(wxT("IDC_CHK_EXPORTSKELECTON")), wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&CDlgMeshExporterOption::OnChkExportSkelectonClicked);
	this->Connect(wxXmlResource::GetXRCID(wxT("IDC_CHKL_MESHOPTION")), wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, (wxObjectEventFunction)&CDlgMeshExporterOption::OnChklMeshOptionClicked);
	this->Connect(wxXmlResource::GetXRCID(wxT("IDC_CHKL_SKELECTONOPTION")), wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, (wxObjectEventFunction)&CDlgMeshExporterOption::OnChklSkelectonOptionClicked);

	return true;
}

void CDlgMeshExporterOption::OnChkExportMeshClicked(wxCommandEvent& event)
{
	m_bExportMesh = event.IsChecked();
	m_pChklMeshOption->Enable(m_bExportMesh);
}

void CDlgMeshExporterOption::OnChkExportSkelectonClicked(wxCommandEvent& event)
{
	m_bExportSkelecton = event.IsChecked();
	m_pChklSkelectonOption->Enable(m_bExportSkelecton);
}

void CDlgMeshExporterOption::OnChklMeshOptionClicked(wxCommandEvent& event)
{
	UpdateVertexFlag();
}

void CDlgMeshExporterOption::OnChklSkelectonOptionClicked(wxCommandEvent& event)
{
	// TODO: 
}

void CDlgMeshExporterOption::LoadConfig(const wxString& strFile)
{
	IOEXmlDocument* pDocument = g_pOEXmlMgr->CreateDocument(strFile.c_str());
	LoadXml(pDocument);
	SAFE_RELEASE(pDocument);
	ResetGUI();
}

void CDlgMeshExporterOption::SaveConfig(const wxString& strFile)
{
	IOEXmlDocument* pDocument = g_pOEXmlMgr->CreateDocument();
	if (SaveXml(pDocument)) pDocument->SaveFile(strFile.c_str());
	SAFE_RELEASE(pDocument);
}

bool CDlgMeshExporterOption::LoadXml(IOEXmlDocument* pDocument)
{
	if (!pDocument) return false;

	IOEXmlNode* pRootNode = pDocument->GetRootNode();
	if (!pRootNode) return false;

	IOEXmlNode* pExportMesh = pRootNode->FirstChild(TS("ExportMesh"));
	if (!pExportMesh) return false;
	pExportMesh->GetAttribute(m_bExportMesh, TS("export"));

	IOEXmlNode* pVertexFlag = pExportMesh->FirstChild(TS("VertexFlag"));
	if (!pVertexFlag) return false;
	pVertexFlag->GetText(m_nVertexFlag);

	IOEXmlNode* pExportSkelecton = pRootNode->FirstChild(TS("ExportSkelecton"));
	if (!pExportSkelecton) return false;
	pExportSkelecton->GetAttribute(m_bExportSkelecton, TS("export"));

	return true;
}

bool CDlgMeshExporterOption::SaveXml(IOEXmlDocument* pDocument)
{
	if (!pDocument) return false;

	IOEXmlNode* pRootNode = pDocument->InsertRootNode(TS("MeshExporter"));
	if (!pRootNode) return false;

	IOEXmlNode* pExportMesh = pRootNode->InsertChild(TS("ExportMesh"));
	if (!pExportMesh) return false;
	pExportMesh->SetAttribute(TS("export"), m_bExportMesh);

	IOEXmlNode* pVertexFlag = pExportMesh->InsertChild(TS("VertexFlag"));
	if (!pVertexFlag) return false;
	pVertexFlag->SetText(m_nVertexFlag);

	IOEXmlNode* pExportSkelecton = pRootNode->InsertChild(TS("ExportSkelecton"));
	if (!pExportSkelecton) return false;
	pExportSkelecton->SetAttribute(TS("export"), m_bExportSkelecton);

	return true;
}

void CDlgMeshExporterOption::UpdateVertexFlag()
{
	m_nVertexFlag = 0;
	if (m_pChklMeshOption->IsChecked(VI_POSITION)) m_nVertexFlag |= VF_POSITION;
	if (m_pChklMeshOption->IsChecked(VI_TEXCOORD0)) m_nVertexFlag |= VF_TEXCOORD0;
	if (m_pChklMeshOption->IsChecked(VI_TEXCOORD1)) m_nVertexFlag |= VF_TEXCOORD1;
	if (m_pChklMeshOption->IsChecked(VI_TEXCOORD2)) m_nVertexFlag |= VF_TEXCOORD2;
	if (m_pChklMeshOption->IsChecked(VI_TEXCOORD3)) m_nVertexFlag |= VF_TEXCOORD3;
	if (m_pChklMeshOption->IsChecked(VI_TEXCOORD4)) m_nVertexFlag |= VF_TEXCOORD4;
	if (m_pChklMeshOption->IsChecked(VI_TEXCOORD5)) m_nVertexFlag |= VF_TEXCOORD5;
	if (m_pChklMeshOption->IsChecked(VI_TEXCOORD6)) m_nVertexFlag |= VF_TEXCOORD6;
	if (m_pChklMeshOption->IsChecked(VI_TEXCOORD7)) m_nVertexFlag |= VF_TEXCOORD7;
	if (m_pChklMeshOption->IsChecked(VI_COLOR)) m_nVertexFlag |= VF_COLOR;
	if (m_pChklMeshOption->IsChecked(VI_NORMAL)) m_nVertexFlag |= VF_NORMAL;
	if (m_pChklMeshOption->IsChecked(VI_TANGENT)) m_nVertexFlag |= VF_TANGENT;
	if (m_pChklMeshOption->IsChecked(VI_BINORMAL)) m_nVertexFlag |= VF_BINORMAL;
	if (m_pChklMeshOption->IsChecked(VI_SKELECTON)) m_nVertexFlag |= VF_SKELECTON;
}

void CDlgMeshExporterOption::ResetGUI()
{
	m_pChkExportMesh->SetValue(m_bExportMesh);
	m_pChkExportSkelecton->SetValue(m_bExportSkelecton);

	m_pChklMeshOption->Enable(m_bExportMesh);
	m_pChklSkelectonOption->Enable(m_bExportSkelecton);

	m_pChklMeshOption->Check(VI_POSITION, m_nVertexFlag & VF_POSITION ? true : false);
	m_pChklMeshOption->Check(VI_TEXCOORD0, m_nVertexFlag & VF_TEXCOORD0 ? true : false);
	m_pChklMeshOption->Check(VI_TEXCOORD1, m_nVertexFlag & VF_TEXCOORD1 ? true : false);
	m_pChklMeshOption->Check(VI_TEXCOORD2, m_nVertexFlag & VF_TEXCOORD2 ? true : false);
	m_pChklMeshOption->Check(VI_TEXCOORD3, m_nVertexFlag & VF_TEXCOORD3 ? true : false);
	m_pChklMeshOption->Check(VI_TEXCOORD4, m_nVertexFlag & VF_TEXCOORD4 ? true : false);
	m_pChklMeshOption->Check(VI_TEXCOORD5, m_nVertexFlag & VF_TEXCOORD5 ? true : false);
	m_pChklMeshOption->Check(VI_TEXCOORD6, m_nVertexFlag & VF_TEXCOORD6 ? true : false);
	m_pChklMeshOption->Check(VI_TEXCOORD7, m_nVertexFlag & VF_TEXCOORD7 ? true : false);
	m_pChklMeshOption->Check(VI_COLOR, m_nVertexFlag & VF_COLOR ? true : false);
	m_pChklMeshOption->Check(VI_NORMAL, m_nVertexFlag & VF_NORMAL ? true : false);
	m_pChklMeshOption->Check(VI_TANGENT, m_nVertexFlag & VF_TANGENT ? true : false);
	m_pChklMeshOption->Check(VI_BINORMAL, m_nVertexFlag & VF_BINORMAL ? true : false);
	m_pChklMeshOption->Check(VI_SKELECTON, m_nVertexFlag & VF_SKELECTON ? true : false);
}
