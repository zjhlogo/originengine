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
#include <OEFmtMesh.h>

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

	typedef std::vector<NODE_INFO> TV_NODE_INFO;

	typedef struct SKIN_tag
	{
		int nBoneIndex;
		float fWeight;
	} SKIN;

	typedef std::vector<SKIN> TV_SKIN;

	typedef std::vector<int> TV_INT;

	typedef struct VERTEX_SLOT_tag
	{
		bool bUsed;
		uint nVertIndex;
		uint nTexIndex;
		TV_INT vClone;

		Point3 pos;
		Point2 tex;
		TV_SKIN Skins;
	} VERTEX_SLOT;

	typedef std::vector<VERTEX_SLOT> TV_VERTEX_SLOT;

	typedef struct FACE_tag
	{
		int nVertIndex[3];
	} FACE;

	typedef std::vector<FACE> TV_FACE;

	typedef struct SKIN_MESH_tag
	{
		tstring strName;
		GMatrix matLocal;
		TV_VERTEX_SLOT vVertSlots;
		TV_FACE vFaces;
	} SKIN_MESH;

	typedef std::vector<SKIN_MESH> TV_SKIN_MESH;

	typedef struct FRAME_INFO_tag
	{
		TimeValue Time;
		GMatrix matAnim;
	} FRAME_INFO;

	typedef std::vector<FRAME_INFO> TV_FRAME_INFO;

	typedef struct BONE_INFO_tag
	{
		tstring strName;
		IGameNode* pNode;
		IGameNode* pParentNode;

		int nID;
		int nParentID;

		GMatrix matLocal;
		TV_FRAME_INFO vFrameInfo;
	} BONE_INFO;

	typedef std::vector<BONE_INFO> TV_BONE_INFO;
	typedef std::map<IGameNode*, int> TM_BONE_INFO;
	typedef std::set<TimeValue> TS_TIME_VALUE;

	typedef struct FILE_VERTEX_tag
	{
		float x, y, z;
		float u, v;
		uchar nBoneIndex[4];
		float fWeight[4];
	} FILE_VERTEX;

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

	bool DumpMesh(SKIN_MESH& SkinMeshOut, IGameNode* pGameNode);
	bool DumpSkin(SKIN_MESH& SkinMeshOut, IGameSkin* pGameSkin);

	bool DumpController(TV_FRAME_INFO& vFrameInfoOut, IGameNode* pGameNode);
	bool DumpPositionController(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);
	bool DumpRotationController(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);
	bool DumpScaleController(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);

	bool DumpMaxStdPosKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);
	bool DumpBipedPosKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);
	bool DumpIndependentPosKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);

	bool DumpMaxStdRotKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);
	bool DumpBipedRotKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);
	bool DumpEulerRotKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);

	bool DumpMaxStdScaleKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);
	bool DumpBipedScaleKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);

	bool DumpConstraintKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);
	bool DumpListKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl);

	void GMatrix2BoneTransform(COEFmtMesh::BONE_TRANSFORM& BoneTrans, const GMatrix& matTrans);
	void SortSkin(TV_SKIN& vSkin);

private:
	TV_NODE_INFO m_vMeshNode;
	TV_NODE_INFO m_vBoneNode;
	TV_SKIN_MESH m_vSkinMesh;
	TV_BONE_INFO m_vBoneInfo;
	TM_BONE_INFO m_vBoneInfoMap;
	TS_TIME_VALUE m_TimeValueSet;

};
#endif // __MESHEXPORTER_H__
