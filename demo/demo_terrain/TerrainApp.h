/*!
 * \file TerrainApp.h
 * \date 24-5-2009 0:10:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __TERRAINAPP_H__
#define __TERRAINAPP_H__

#include "../common/BaseApp.h"
#include "Terrain.h"

class CTerrainApp : public CBaseApp
{
public:
	CTerrainApp();
	virtual ~CTerrainApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();

	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

private:
	IOEModel* m_pModel;
	CTerrain* m_pTerrain;

};
#endif // __TERRAINAPP_H__
