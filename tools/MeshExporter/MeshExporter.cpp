/*!
 * \file MeshExporter.cpp
 * \date 1-8-2009 10:36:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshExporter.h"

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
	ExportController(pGameNode);

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
			IGameKeyTab PosKey;
			if (pGameControl->GetBezierKeys(PosKey, IGAME_POS))
			{
				// TODO: export Bezier Key
			}
			else if (pGameControl->GetLinearKeys(PosKey, IGAME_POS))
			{
				// TODO: export Linear Key
			}
			else
			{
				// TODO: 
			}
		}
		break;
	case IGameControl::IGAME_POS_CONSTRAINT:
		{
			// TODO: export constraint controller
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// TODO: export list controller
		}
		break;
	case IGameControl::IGAME_INDEPENDENT_POS:
		{
			// TODO: export independent controller
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

bool CMeshExporter::ExportRotationController(IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_ROT);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			IGameKeyTab RotKey;
			if (pGameControl->GetTCBKeys(RotKey, IGAME_ROT))
			{
				// TODO: export TCB Keys
			}
			else if (pGameControl->GetLinearKeys(RotKey, IGAME_ROT))
			{
				// TODO: export Linear Keys
			}
			else if (pGameControl->GetBezierKeys(RotKey, IGAME_ROT))
			{
				// TODO: export Bezier Keys
			}
			else
			{
				// TODO: 
			}
		}
		break;
	case IGameControl::IGAME_EULER:
		{
			// TODO: export Euler controller
		}
		break;
	case IGameControl::IGAME_ROT_CONSTRAINT:
		{
			// TODO: export constraint controller
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// TODO: export list controller
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
			IGameKeyTab ScaleKey;
			if (pGameControl->GetBezierKeys(ScaleKey, IGAME_SCALE))
			{
				// TODO: export Bezier Keys
			}
			else
			{
				// TODO: 
			}
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
