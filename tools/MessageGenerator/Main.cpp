/*!
 * \file Main.cpp
 * \date 8-18-2010 0:12:34
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Main.h"
#include "../common/OEModuleLoader.h"
#include "MessageGenerator.h"
#include <libOEBase/OEOS.h>
#include <iostream>
#include <string>

void PrintUsage()
{
	static const tchar* s_szUsage = 
		TS("Message Generator version 1.0\n")
		TS("author: zjhlogo, zjhlogo@gmail.com\n")
		TS("Usage: MessageGenerator.exe (xml file name) (module name)\n")
		TS("example: MessageGenerator.exe OEMsgList.xml OEMsgList\n");

	std::string strUsage;
	COEOS::tchar2char(strUsage, s_szUsage);
	std::cout << strUsage << std::endl;
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		PrintUsage();
		return 0;
	}

	if (COEModuleLoader::Initialize())
	{
		tstring strXmlFile;
		tstring strModule;

		COEOS::char2tchar(strXmlFile, argv[1]);
		COEOS::char2tchar(strModule, argv[2]);

		CMessageGenerator gen;
		gen.Convert(strXmlFile, strModule);
	}

	COEModuleLoader::Terminate();
	return 0;
}
