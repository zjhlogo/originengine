/*!
 * \file MeshConvMain.cpp
 * \date 30-7-2009 20:14:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshConvMain.h"
#include "ConvMgr.h"

#include <OEOS.h>

bool CheckParameter(int argc, char** argv)
{
	// TODO: 
	if (argc < 2) return false;
	return true;
}

int main(int argc, char** argv)
{
	if (!COEOS::Initialize(COEOS::IS_FILE)) return 0;

	if (!CheckParameter(argc, argv))
	{
		COEOS::Terminate();
		return 0;
	}

	tstring strFileIn;
	if (!COEOS::char2tchar(strFileIn, argv[1]))
	{
		COEOS::Terminate();
		return 0;
	}

	if (!CConvMgr::Get().Initialized())
	{
		COEOS::Terminate();
		return 0;
	}

	tstring strFileOut;
	COEOS::GetFileName(strFileOut, strFileOut);
	strFileOut += t(".mesh");

	CConvMgr::Get().DoConvert(strFileIn, strFileOut);

	COEOS::Terminate();
	return 0;
}
