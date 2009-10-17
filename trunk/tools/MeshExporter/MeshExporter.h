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
#include <IGame/IGameModifier.h>

#include <vector>
#include <set>
#include <map>

#include <IOEFileMgr.h>

class CMeshExporter : public SceneExport
{
public:
	enum CONST_DEFINE
	{
		CURRENT_VERSION = 100,
	};

	typedef struct NODE_INFO_tag
	{
		IGameNode* pGameNode;
		IGameNode* pParentGameNode;
	} NODE_INFO;

	typedef std::vector<NODE_INFO> VNODE_INFO;

	typedef struct SKIN_tag
	{
		int nBoneIndex;
		float fWeight;
	} SKIN;

	typedef std::vector<SKIN> VSKIN;

	typedef struct VERTEX_tag
	{
		Point3 pos;
		Point2 tex;
		//float nx, ny, nz;
		VSKIN Skins;
	} VERTEX;

	typedef std::vector<VERTEX> VVERTEX;

	typedef struct FACE_tag
	{
		int nVertIndex[3];
	} FACE;

	typedef std::vector<FACE> VFACE;

	typedef struct SKIN_MESH_tag
	{
		tstring strName;
		GMatrix matLocal;
		VVERTEX vVerts;
		VFACE vFaces;
	} SKIN_MESH;

	typedef std::vector<SKIN_MESH> VSKIN_MESH;

	typedef struct FRAME_INFO_tag
	{
		TimeValue Time;
		GMatrix matAnim;
	} FRAME_INFO;

	typedef std::vector<FRAME_INFO> VFRAME_INFO;

	typedef struct BONE_INFO_tag
	{
		tstring strName;
		int nNodeID;
		int nParentNodeID;

		int nID;
		int nParentID;

		GMatrix matLocal;
		VFRAME_INFO vFrameInfo;
	} BONE_INFO;

	typedef std::vector<BONE_INFO> VBONE_INFO;
	typedef std::map<int, int> BONE_INFO_MAP;

	typedef std::set<TimeValue> TIME_VALUE_SET;

	typedef struct FILE_BLOCK_tag
	{
		uint nOffset;
		uint nSize;
	} FILE_BLOCK;

	typedef std::vector<FILE_BLOCK> VFILE_BLOCK;

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

	void Cleanup();

	bool CollectNodes(IGameNode* pGameNode, IGameNode* pParentGameNode = NULL);

	bool BuildMeshsInfo();
	bool BuildBonesInfo();

	bool SaveToFile(const tstring& strFileName);

	bool DumpSkinMesh(SKIN_MESH& SkinMeshOut, IGameNode* pGameNode);
	bool DumpSkinVerts(SKIN_MESH& SkinMeshOut, IGameSkin* pGameSkin);

	bool DumpController(VFRAME_INFO& vFrameInfoOut, IGameNode* pGameNode);
	bool DumpPositionController(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);
	bool DumpRotationController(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);
	bool DumpScaleController(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);

	bool DumpMaxStdPosKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);
	bool DumpBipedPosKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);
	bool DumpIndependentPosKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);

	bool DumpMaxStdRotKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);
	bool DumpBipedRotKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);
	bool DumpEulerRotKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);

	bool DumpMaxStdScaleKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);
	bool DumpBipedScaleKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);

	bool DumpConstraintKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);
	bool DumpListKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl);

private:
	VNODE_INFO m_vMeshNode;
	VNODE_INFO m_vBoneNode;
	VSKIN_MESH m_vSkinMesh;
	VBONE_INFO m_vBoneInfo;
	BONE_INFO_MAP m_vBoneInfoMap;
	TIME_VALUE_SET m_TimeValueSet;

};
#endif // __MESHEXPORTER_H__
