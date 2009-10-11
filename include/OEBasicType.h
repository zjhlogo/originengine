/*!
 * \file OEBasicType.h
 * \date 24-5-2009 9:20:34
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBASICTYPE_H__
#define __OEBASICTYPE_H__

#include <string>
#include "OEConfig.h"

//typedef bool						bool;

//typedef char						char;
typedef unsigned char				uchar;

//typedef short						short;
typedef unsigned short				ushort;

//typedef int						int;
typedef unsigned int				uint;

typedef __int64						int64;
typedef unsigned __int64			uint64;

//typedef float						float;

//typedef NULL						NULL;

#ifdef _UNICODE
	#define t(x) L ## x
	typedef wchar_t tchar;
#else
	#define t(x) x
	typedef char tchar;
#endif // _UNICODE

typedef std::basic_string<tchar>	tstring;

#define SAFE_DELETE(x) if (x) {delete x; x = NULL;}
#define SAFE_DELETE_ARRAY(x) if (x) {delete[] x; x = NULL;}
#define SAFE_RELEASE(x) if (x) {x->Release(); x = NULL;}

extern const tstring EMPTY_STRING;

#endif // __OEBASICTYPE_H__
