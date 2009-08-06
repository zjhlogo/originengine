/*!
 * \file MeshExporter.cpp
 * \date 1-8-2009 10:36:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshExporter.h"
#include <OEOS.h>

CMeshExporter::CMeshExporter()
{
	Init();
}

CMeshExporter::~CMeshExporter()
{
	Destroy();
}

void CMeshExporter::Init()
{
	m_pFile = NULL;
	m_pIGame = NULL;
}

void CMeshExporter::Destroy()
{
	SAFE_RELEASE(m_pFile);
}

int CMeshExporter::ExtCount()
{
	return 1;
}

const TCHAR* CMeshExporter::Ext(int n)
{
	return _T("mesh");
}

const TCHAR* CMeshExporter::LongDesc()
{
	return _T("Export to .mesh file format");
}

const TCHAR* CMeshExporter::ShortDesc()
{
	return _T(".mesh file format");
}

const TCHAR* CMeshExporter::AuthorName()
{
	return _T("zjhlogo");
}

const TCHAR* CMeshExporter::CopyrightMessage()
{
	return _T("Copyright (c) 2009, zjhlogo All rights reserved");
}

const TCHAR* CMeshExporter::OtherMessage1()
{
	return _T("Other message1");
}

const TCHAR* CMeshExporter::OtherMessage2()
{
	return _T("Other message2");
}

unsigned int CMeshExporter::Version()
{
	return CURRENT_VERSION;
}

void CMeshExporter::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, _T("Mesh Exporter"), _T("Copyright (c) 2009, zjhlogo All rights reserved"), MB_OK);
}

int CMeshExporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts /* = FALSE */, DWORD options /* = 0 */)
{
	m_vVerts.clear();

	SAFE_RELEASE(m_pFile);
	m_pFile = g_pOEFileMgr->OpenFile(name, IOEFile::OFF_WRITE);
	if (!m_pFile) return FALSE;

	m_pIGame = GetIGameInterface();
	IGameConversionManager* pGameConvMgr = GetConversionManager();
	pGameConvMgr->SetCoordSystem(IGameConversionManager::IGAME_D3D);
	m_pIGame->InitialiseIGame();

	int nNodeCount = m_pIGame->GetTopLevelNodeCount();
	for (int i = 0; i < nNodeCount; ++i)
	{
		IGameNode* pGameNode = m_pIGame->GetTopLevelNode(i);
		ExportNode(pGameNode);
	}

	m_pIGame->ReleaseIGame();
	m_pIGame = NULL;

	SAFE_RELEASE(m_pFile);
	return TRUE;
}

bool CMeshExporter::ExportNode(IGameNode* pGameNode, int nDepth /* = 0 */)
{
	if (!pGameNode) return false;

	// export object
	ExportMesh(pGameNode);

	// export controller
	m_TimeValueSet.clear();
	ExportController(pGameNode);
	DumpTimeValueSet(m_TimeValueSet);

	// export child
	int nChildCount = pGameNode->GetChildCount();
	for (int i = 0; i < nChildCount; ++i)
	{
		bool bOK = ExportNode(pGameNode->GetNodeChild(i), nDepth+1);
		// TODO: check bOK
	}

	return true;
}

bool CMeshExporter::ExportMesh(IGameNode* pGameNode)
{
	IGameObject* pGameObject = pGameNode->GetIGameObject();
	if (pGameObject->GetIGameType() != IGameObject::IGAME_MESH) return false;

	IGameMesh* pGameMesh = (IGameMesh*)pGameObject;
	int nVertsCount = pGameMesh->GetNumberOfVerts();

	// export vertices
	for(int i = 0; i < nVertsCount; ++i)
	{
		VERTEX Vert;
		bool bOK = pGameMesh->GetVertex(i, Vert.pos, true);
		assert(bOK);
		m_vVerts.push_back(Vert);

		// TODO: more
	}

	return true;
}

bool CMeshExporter::ExportController(IGameNode* pGameNode)
{
	IGameControl* pGameControl = pGameNode->GetIGameControl();

	// export position controller
	if (pGameControl->IsAnimated(IGAME_POS)) ExportPositionController(pGameControl);

	// export rotation controller
	if (pGameControl->IsAnimated(IGAME_ROT)) ExportRotationController(pGameControl);

	// export scale controller
	if (pGameControl->IsAnimated(IGAME_SCALE)) ExportScaleController(pGameControl);

	return true;
}

bool CMeshExporter::ExportPositionController(IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_POS);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std pos key
			ExportMaxStdPosKey(pGameControl);
		}
		break;
	case IGameControl::IGAME_POS_CONSTRAINT:
		{
			// export constraint controller
			ExportConstraintKey(pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			ExportListKey(pGameControl);
		}
		break;
	case IGameControl::IGAME_INDEPENDENT_POS:
		{
			// export independent controller
			ExportIndependentPosKey(pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::ExportRotationController(IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_ROT);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std rot key
			ExportMaxStdRotKey(pGameControl);
		}
		break;
	case IGameControl::IGAME_EULER:
		{
			// export Euler controller
			ExportEulerRotKey(pGameControl);
		}
		break;
	case IGameControl::IGAME_ROT_CONSTRAINT:
		{
			// export constraint controller
			ExportConstraintKey(pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			ExportListKey(pGameControl);
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::ExportScaleController(IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_SCALE);

	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export scale key
			ExportMaxStdScaleKey(pGameControl);
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::ExportMaxStdPosKey(IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(PosKey[i].t);
			// TODO: time = PosKey[i].t;
			// TODO: position = PosKey[i].bezierKey.pval;
		}
	}
	else if (pGameControl->GetLinearKeys(PosKey, IGAME_POS))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(PosKey[i].t);
			// TODO: time = PosKey[i].t;
			// TODO: position = PosKey[i].linearKey.pval;
		}
	}
	else
	{
		// TODO: 
	}

	return true;
}

bool CMeshExporter::ExportIndependentPosKey(IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_X))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(PosKey[i].t);
			// TODO: time = PosKey[i].t;
			// TODO: position = PosKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_X))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(PosKey[i].t);
			// TODO: time = PosKey[i].t;
			// TODO: position = PosKey[i].linearKey.fval;
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Y))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(PosKey[i].t);
			// TODO: time = PosKey[i].t;
			// TODO: position = PosKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Y))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(PosKey[i].t);
			// TODO: time = PosKey[i].t;
			// TODO: position = PosKey[i].linearKey.fval;
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Z))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(PosKey[i].t);
			// TODO: time = PosKey[i].t;
			// TODO: position = PosKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Z))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(PosKey[i].t);
			// TODO: time = PosKey[i].t;
			// TODO: position = PosKey[i].linearKey.fval;
		}
	}

	return true;
}

bool CMeshExporter::ExportMaxStdRotKey(IGameControl* pGameControl)
{
	IGameKeyTab RotKey;
	if (pGameControl->GetBezierKeys(RotKey, IGAME_ROT))
	{
		// export Bezier Keys
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: Quat = RotKey[i].bezierKey.qval;
		}
	}
	else if (pGameControl->GetLinearKeys(RotKey, IGAME_ROT))
	{
		// export Linear Keys
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: Quat = RotKey[i].linearKey.qval;
		}
	}
	else if (pGameControl->GetTCBKeys(RotKey, IGAME_ROT))
	{
		// export TCB Keys
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: AngAxis = RotKey[i].tcbKey.aval;
		}
	}
	else
	{
		// TODO: 
	}

	return true;
}

bool CMeshExporter::ExportEulerRotKey(IGameControl* pGameControl)
{
	IGameKeyTab RotKey;

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_X))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: RotKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_X))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: RotKey[i].linearKey.fval;
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Y))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: RotKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Y))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: RotKey[i].linearKey.fval;
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Z))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: RotKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Z))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(RotKey[i].t);
			// TODO: time = RotKey[i].t;
			// TODO: RotKey[i].linearKey.fval;
		}
	}

	return true;
}

bool CMeshExporter::ExportMaxStdScaleKey(IGameControl* pGameControl)
{
	IGameKeyTab ScaleKey;
	if (pGameControl->GetBezierKeys(ScaleKey, IGAME_SCALE))
	{
		int nCount = ScaleKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			m_TimeValueSet.insert(ScaleKey[i].t);
			// TODO: time = ScaleKey[i].t;
			// TODO: ScaleKey[i].bezierKey.sval;
		}
	}
	else
	{
		// TODO: 
	}

	return true;
}

bool CMeshExporter::ExportConstraintKey(IGameControl* pGameControl)
{
	IGameConstraint* pGameConstraint = pGameControl->GetConstraint(IGAME_POS);

	int nConstraintCount = pGameConstraint->NumberOfConstraintNodes();
	for(int i = 0; i < nConstraintCount; ++i)
	{
		float fWeight = pGameConstraint->GetConstraintWeight(i);
		int nNodeID = pGameConstraint->GetConstraintNodes(i)->GetNodeID();

		// TODO: export constraint
	}

	IPropertyContainer* pPropertyContainer = pGameConstraint->GetIPropertyContainer();
	int nPropertyCount = pPropertyContainer->GetNumberOfProperties();

	for(int i = 0; i < nPropertyCount; ++i)
	{
		// TODO: dump property
	}

	return true;
}

bool CMeshExporter::ExportListKey(IGameControl* pGameControl)
{
	int nCount = pGameControl->GetNumOfListSubControls(IGAME_POS);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_POS);
		bool bOK = ExportPositionController(pSubGameControl);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_ROT);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_ROT);
		bool bOK = ExportRotationController(pSubGameControl);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_SCALE);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_SCALE);
		bool bOK = ExportScaleController(pSubGameControl);
		// TODO: check bOK
	}

	return true;
}

bool CMeshExporter::ExportSampleKey(IGameControl* pGameControl)
{
	// TODO: 
	return false;
}

void CMeshExporter::DumpTimeValueSet(const TIME_VALUE_SET& TimeValueSet)
{
	for (TIME_VALUE_SET::const_iterator it = TimeValueSet.begin(); it != TimeValueSet.end(); ++it)
	{
		tstring strOut;
		COEOS::strformat(strOut, _T("%d, "), (*it));
		m_pFile->Write(strOut.c_str(), (uint)(strOut.length()*sizeof(tchar)));
	}

	m_pFile->Write(_T("\n"), sizeof(tchar));
}
