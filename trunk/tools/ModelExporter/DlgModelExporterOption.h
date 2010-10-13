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
	int GetVertexFlag() const;

	void LoadConfig(const wxString& strFile);
	void SaveConfig(const wxString& strFile);

	bool IsExportMesh() {return m_bExportMesh;};
	bool IsExportSkelecton() {return m_bExportSkelecton;};

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
