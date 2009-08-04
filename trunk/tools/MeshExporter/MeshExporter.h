/*!
 * \file MeshExporter.h
 * \date 1-8-2009 10:36:32
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MESHEXPORTER_H__
#define __MESHEXPORTER_H__

#include <max.h>
#include <IGame/IGame.h>

#include <vector>
#include <IOEFileMgr.h>

class CMeshExporter : public SceneExport
{
public:
	enum CONST_DEFINE
	{
		CURRENT_VERSION = 100,
	};

	typedef struct BONE_tag
	{
		int nBoneIndex;
		float fWeight;
	} BONE;

	typedef std::vector<BONE> VBONE;

	typedef struct VERTEX_tag
	{
		Point3 pos;
		float u, v;
		float nx, ny, nz;
		VBONE Bones;
	} VERTEX;

	typedef std::vector<VERTEX> VVERTEX;

public:
	CMeshExporter();
	virtual ~CMeshExporter();

	virtual int ExtCount();						// Number of extensions supported
	virtual const TCHAR* Ext(int n);			// Extension #n (i.e. "3DS")
	virtual const TCHAR* LongDesc();			// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR* ShortDesc();			// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR* AuthorName();			// ASCII Author name
	virtual const TCHAR* CopyrightMessage();	// ASCII Copyright message
	virtual const TCHAR* OtherMessage1();		// Other message #1
	virtual const TCHAR* OtherMessage2();		// Other message #2
	virtual unsigned int Version();				// Version number * 100 (i.e. v3.01 = 301)
	virtual void ShowAbout(HWND hWnd);			// Show DLL's "About..." box
	virtual int DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts = FALSE, DWORD options = 0);	// Export file

private:
	void Init();
	void Destroy();

	bool ExportNode(IGameNode* pGameNode, int nDepth = 0);
	bool ExportMesh(IGameNode* pGameNode);

	bool ExportController(IGameNode* pGameNode);
	bool ExportPositionController(IGameControl* pGameControl);
	bool ExportRotationController(IGameControl* pGameControl);
	bool ExportScaleController(IGameControl* pGameControl);

private:
	IOEFile* m_pFile;
	IGameScene * m_pIGame;

	VVERTEX m_vVerts;

};
#endif // __MESHEXPORTER_H__
