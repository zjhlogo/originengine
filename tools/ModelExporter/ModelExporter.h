/*!
 * \file ModelExporter.h
 * \date 1-3-2010 21:00:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MODELEXPORTER_H__
#define __MODELEXPORTER_H__

#include <max.h>
#include <IGame/IGame.h>
#include <IGame/IGameModifier.h>

#include <OEBase/IOEFileMgr.h>
#include <OECore/OEFmtMesh.h>
#include <OECore/OEFmtSkeleton.h>

#include <vector>
#include <set>
#include <map>

class CDlgModelExporterOption;
class wxWindow;

class CModelExporter : public SceneExport
{
public:
	enum CONST_DEFINE
	{
		CURRENT_VERSION = 100,
	};

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
		KFM_POS			= KFM_POS_X | KFM_POS_Y | KFM_POS_Z,
		KFM_SCALE		= KFM_SCALE_X | KFM_SCALE_Y | KFM_SCALE_Z,
		KFM_ROT			= KFM_ROT_X | KFM_ROT_Y | KFM_ROT_Z,
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

	typedef struct FILE_VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		float tx, ty, tz;
		uchar nBoneIndex[4];
		float fWeight[4];
	} FILE_VERTEX;

	// bone information
	typedef struct KEYFRAME_ROT_tag
	{
		TimeValue time;
		uint nMask;
		CVector3 vRot;
		CQuaternion qRot;
	} KEYFRAME_ROT;

	typedef struct KEYFRAME_POS_tag
	{
		TimeValue time;
		uint nMask;
		CVector3 vPos;
	} KEYFRAME_POS;

	typedef struct KEYFRAME_SCALE_tag
	{
		TimeValue time;
		uint nMask;
		CVector3 vScale;
	} KEYFRAME_SCALE;

	typedef std::map<TimeValue, KEYFRAME_ROT> TM_KEYFRAME_ROT;
	typedef std::map<TimeValue, KEYFRAME_POS> TM_KEYFRAME_POS;
	typedef std::map<TimeValue, KEYFRAME_SCALE> TM_KEYFRAME_SCALE;

	typedef struct BONE_INFO_tag
	{
		tstring strName;
		IGameNode* pNode;
		IGameNode* pParentNode;

		int nID;
		int nParentID;

		CMatrix4x4 matLocal;

		TM_KEYFRAME_ROT FrameRot;
		TM_KEYFRAME_POS FramePos;
		TM_KEYFRAME_SCALE FrameScale;
	} BONE_INFO;

	typedef std::vector<BONE_INFO> TV_BONE_INFO;
	typedef std::map<IGameNode*, int> TM_BONE_INFO;

public:
	CModelExporter();
	virtual ~CModelExporter();

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

	bool SaveMeshFile(const tstring& strFileName);
	bool SaveSkeletonFile(const tstring& strFileName);

	bool DumpMesh(SKIN_MESH& SkinMeshOut, IGameNode* pGameNode);
	bool DumpSkin(SKIN_MESH& SkinMeshOut, IGameSkin* pGameSkin);

	bool DumpController(BONE_INFO& BoneInfo, IGameNode* pGameNode);
	bool DumpPositionController(BONE_INFO& BoneInfo, IGameControl* pGameControl);
	bool DumpRotationController(BONE_INFO& BoneInfo, IGameControl* pGameControl);
	bool DumpScaleController(BONE_INFO& BoneInfo, IGameControl* pGameControl);

	bool DumpMaxStdPosKey(BONE_INFO& BoneInfo, IGameControl* pGameControl);
	bool DumpIndependentPosKey(BONE_INFO& BoneInfo, IGameControl* pGameControl);

	bool DumpMaxStdRotKey(BONE_INFO& BoneInfo, IGameControl* pGameControl);
	bool DumpEulerRotKey(BONE_INFO& BoneInfo, IGameControl* pGameControl);

	bool DumpMaxStdScaleKey(BONE_INFO& BoneInfo, IGameControl* pGameControl);

	bool DumpConstraintKey(BONE_INFO& BoneInfo, IGameControl* pGameControl);
	bool DumpListKey(BONE_INFO& BoneInfo, IGameControl* pGameControl);
	bool DumpSampleKey(BONE_INFO& BoneInfo, IGameControl* pGameControl, IGameControlType eType);

	KEYFRAME_POS* GetKeyFrame(TM_KEYFRAME_POS& FramePos, TimeValue time);
	KEYFRAME_SCALE* GetKeyFrame(TM_KEYFRAME_SCALE& FrameScale, TimeValue time);
	KEYFRAME_ROT* GetKeyFrame(TM_KEYFRAME_ROT& FrameRot, TimeValue time);

	bool InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, KEY_FRAME_MASK eMask, float fValue);
	bool InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, KEY_FRAME_MASK eMask, const Point3& vValue);
	bool InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, const Quat& qValue);
	bool InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, const GMatrix& matValue);

	void MaxMat2OeMat(CMatrix4x4& matOut, const GMatrix& matIn);
	void OeMat2MaxMat(GMatrix& matOut, const CMatrix4x4& matIn);

	void MaxQuat2OeQuat(CQuaternion& qOut, const Quat& qIn);
	void OeQuat2MaxQuat(Quat& qOut, const CQuaternion& qIn);

	void MaxVec2OeVec(CVector3& vOut, const Point3& vIn);
	void OeVec2MaxVec(Point3& vOut, const CVector3& vIn);

	void MaxEular2OeEular(CVector3& vOut, const Point3& vIn);
	void OeEular2MaxEular(Point3& vOut, const CVector3& vIn);

	void SortSkin(TV_SKIN& vSkin);

	bool ReadConfig();
	bool SaveConfig();

	bool ShowOptionDialog();

private:
	TV_NODE_INFO m_vMeshNode;
	TV_NODE_INFO m_vBoneNode;
	TV_SKIN_MESH m_vSkinMesh;
	TV_BONE_INFO m_vBoneInfo;
	TM_BONE_INFO m_vBoneInfoMap;
	CVector3 m_vBoundingBoxMin;
	CVector3 m_vBoundingBoxMax;
	Interface* m_pInterface;

	bool m_bExportMesh;
	bool m_bExportBone;

	bool m_bOptimizeRotation;
	float m_fOptimizeRotation;

	bool m_bOptimizePosition;
	float m_fOptimizePosition;

	bool m_bOptimizeScale;
	float m_fOptimizeScale;

	wxWindow* m_pParentWindow;
	CDlgModelExporterOption* m_pDlgModelExpOption;

};
#endif // __MODELEXPORTER_H__
