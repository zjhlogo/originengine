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
	typedef std::set<int> TS_INT;

	typedef struct VERTEX_SLOT_tag
	{
		bool bUsed;
		uint nVertIndex;
		uint nTexIndex;
		TV_INT vClone;

		Point3 pos;
		TS_INT vNormalIndex;
		TS_INT vTangentIndex;
		Point3 normal;
		Point3 tangent;
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

	enum KEY_FRAME_MASK
	{
		KFM_UNKNOWN		= 0,
		KFM_POS_X		= 0x00000001,
		KFM_POS_Y		= 0x00000002,
		KFM_POS_Z		= 0x00000004,
		KFM_SCALE_X		= 0x00000008,
		KFM_SCALE_Y		= 0x00000010,
		KFM_SCALE_Z		= 0x00000020,
		KFM_ROT_X		= 0x00000040,
		KFM_ROT_Y		= 0x00000080,
		KFM_ROT_Z		= 0x00000100,
		KFM_QUAT		= 0x00000200,
		KFM_MATRIX		= 0x00000400,
		KFM_POS			= KFM_POS_X | KFM_POS_Y | KFM_POS_Z,
		KFM_SCALE		= KFM_SCALE_X | KFM_SCALE_Y | KFM_SCALE_Z,
		KFM_ROT			= KFM_ROT_X | KFM_ROT_Y | KFM_ROT_Z,
	};

	typedef struct KEY_FRAME_tag
	{
		TimeValue time;
		uint nMask;
		CVector3 vPos;
		CVector3 vScale;
		CVector3 vRot;
		CQuaternion rRot;
		CMatrix4x4 matFull;
	} KEY_FRAME;

	typedef std::map<TimeValue, KEY_FRAME> TM_KEY_FRAME;

	typedef struct FILE_VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		float tx, ty, tz;
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
	bool DumpPositionController(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);
	bool DumpRotationController(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);
	bool DumpScaleController(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);

	bool DumpMaxStdPosKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);
	bool DumpIndependentPosKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);

	bool DumpMaxStdRotKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);
	bool DumpEulerRotKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);

	bool DumpMaxStdScaleKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);

	bool DumpConstraintKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);
	bool DumpListKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl);
	bool DumpSampleKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl, IGameControlType eType);

	KEY_FRAME* FindKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time);
	bool InsertKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time, KEY_FRAME_MASK eMask, float fValue);
	bool InsertKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time, KEY_FRAME_MASK eMask, const Point3& vValue);
	bool InsertKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time, KEY_FRAME_MASK eMask, const Quat& qValue);
	bool InsertKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time, KEY_FRAME_MASK eMask, const GMatrix& matValue);

	void GMatrix2CMatrix4x4(CMatrix4x4& matOut, const GMatrix& matIn);
	void CMatrix4x42GMatrix(GMatrix& matOut, const CMatrix4x4& matIn);
	void GMatrix2BoneTransform(COEFmtMesh::BONE_TRANSFORM& BoneTrans, const GMatrix& matTrans);
	void SortSkin(TV_SKIN& vSkin);

private:
	TV_NODE_INFO m_vMeshNode;
	TV_NODE_INFO m_vBoneNode;
	TV_SKIN_MESH m_vSkinMesh;
	TV_BONE_INFO m_vBoneInfo;
	TM_BONE_INFO m_vBoneInfoMap;
	TM_KEY_FRAME m_TimeValueSet;
	Interface* m_pInterface;

};
#endif // __MESHEXPORTER_H__
