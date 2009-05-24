/*!
 * \file OEBasicType.h
 * \date 24-5-2009 9:20:34
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBASICTYPE_H__
#define __OEBASICTYPE_H__

#include <tchar.h>
#include <string>

#include "OEConfig.h"

//typedef bool						bool;

//typedef char						char;
typedef unsigned char				uchar;

//typedef short						short;
typedef unsigned short				ushort;

//typedef int						int;
typedef unsigned int				uint;

//typedef float						float;

typedef TCHAR						tchar;

typedef std::basic_string<TCHAR>	tstring;

//typedef NULL						NULL;

#define SAFE_DELETE(x) if (x) {delete x; x = NULL;}
#define SAFE_DELETE_ARRAY(x) if (x) {delete[] x; x = NULL;}
#define SAFE_RELEASE(x) if (x) {x->Release(); x = NULL;}

#endif // __OEBASICTYPE_H__
