/*!
 * \file MeshApp.h
 * \date 31-7-2009 20:41:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MESHAPP_H__
#define __MESHAPP_H__

#include "../common/BaseApp.h"
#include "Mesh.h"

class CMeshApp : public CBaseApp
{
public:
	CMeshApp();
	virtual ~CMeshApp();

	virtual bool Initialize();
	virtual void Terminate();

private:
	void Init();
	void Destroy();

private:
	CMesh* m_pMesh;

};

#endif // __MESHAPP_H__
