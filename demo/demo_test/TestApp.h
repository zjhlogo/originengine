/*!
 * \file TestApp.h
 * \date 24-5-2009 0:10:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __TESTAPP_H__
#define __TESTAPP_H__

#include <IOEApp.h>
#include <IOEVertDecl.h>

class CTestApp : public IOEApp
{
public:
	typedef struct VERTEX_tag
	{
		float x, y, z;
		uint nColor;
	} VERTEX;

public:
	CTestApp();
	virtual ~CTestApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

private:
	IOEVertDecl* m_pDecl;

};

#endif // __TESTAPP_H__
