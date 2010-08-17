/*!
 * \file DlgModelExporterOption.h
 * \date 1-3-2010 20:59:07
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __DLGMODELEXPORTEROPTION_H__
#define __DLGMODELEXPORTEROPTION_H__

#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>

#include <OEBase/IOEXmlMgr.h>

class CDlgModelExporterOption : public wxDialog
{
public:
	enum VERTEX_FLAG
	{
		VF_POSITION		= 0x00000001,
		VF_TEXCOORD0	= 0x00000002,
		VF_TEXCOORD1	= 0x00000004,
		VF_TEXCOORD2	= 0x00000008,
		VF_TEXCOORD3	= 0x00000010,
		VF_TEXCOORD4	= 0x00000020,
		VF_TEXCOORD5	= 0x00000040,
		VF_TEXCOORD6	= 0x00000080,
		VF_TEXCOORD7	= 0x00000100,
		VF_COLOR		= 0x00000200,
		VF_NORMAL		= 0x00000400,
		VF_TANGENT		= 0x00000800,
		VF_BINORMAL		= 0x00001000,
		VF_SKELECTON	= 0x00002000,
	};

	enum VERTEX_INDEX
	{
		VI_POSITION = 0,
		VI_TEXCOORD0,
		VI_TEXCOORD1,
		VI_TEXCOORD2,
		VI_TEXCOORD3,
		VI_TEXCOORD4,
		VI_TEXCOORD5,
		VI_TEXCOORD6,
		VI_TEXCOORD7,
		VI_COLOR,
		VI_NORMAL,
		VI_TANGENT,
		VI_BINORMAL,
		VI_SKELECTON,
		VI_NUM,
	};

public:
	CDlgModelExporterOption();
	virtual ~CDlgModelExporterOption();

	bool Initialize(wxWindow* pParent);

	void LoadConfig(const wxString& strFile);
	void SaveConfig(const wxString& strFile);

private:
	void OnChkExportMeshClicked(wxCommandEvent& event);
	void OnChkExportSkelectonClicked(wxCommandEvent& event);
	void OnChklMeshOptionClicked(wxCommandEvent& event);
	void OnChklSkelectonOptionClicked(wxCommandEvent& event);

	bool LoadXml(IOEXmlDocument* pDocument);
	bool SaveXml(IOEXmlDocument* pDocument);

	void UpdateVertexFlag();
	void ResetGUI();

private:
	wxCheckBox* m_pChkExportMesh;
	wxCheckBox* m_pChkExportSkelecton;

	wxCheckListBox* m_pChklMeshOption;
	wxCheckListBox* m_pChklSkelectonOption;

	bool m_bExportMesh;
	bool m_bExportSkelecton;
	int m_nVertexFlag;

};
#endif // __DLGMODELEXPORTEROPTION_H__
