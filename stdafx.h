#ifndef _STDAFX_H
#define _STDAFX_H

#if defined(_WINDOWS)
#pragma once

#define _WIN32_LEAN_AND_MEAN

#ifndef STRICT
#define STRICT
#endif

#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <winsock2.h>
#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <strsafe.h>
#include <CommCtrl.h>
#include <shlobj.h>
#include <math.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <Ole2.h>
#include <OleIdl.h>
#include <ShObjIdl.h>
#include <assert.h>
#include <setjmp.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <sys/types.h>
#include <mmsystem.h>
#include <stddef.h>
#include <time.h>


#ifdef __cplusplus
#include <gdiplus.h>
using namespace Gdiplus;
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
using namespace ATL;
#include <map>
#include <list>
#include <vector>
#include <string>

namespace std
{
#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif
}
#endif

#elif defined(_LINUX)
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stddef.h>
#include <time.h>
#include <ctype.h>
#ifdef _UNICODE
#include <wchar.h>
#endif
#ifdef WXVER
#include <wx/wx.h>
#undef _T
#endif

typedef void *LPVOID;
#define MAX_PATH 256
#ifndef _T
#ifdef _UNICODE
#define _T(z) L ## z
#define TCHAR wchar_t
typedef const wchar_t *LPCTSTR;
typedef wchar_t *LPTSTR;

#define _tcscpy _tcscpy_s
#define _tcscpy_s std::wcscpy
#define _tcsncpy std::wcsncpy
//TODO: fix this to actually properly pass the size
#define _tprintf_s(buffer, format, ...) swprintf(buffer, MAX_PATH, format,  __VA_ARGS__)
#define _tprintf _tprintf_s
#define _tcsicmp wcscasecmp
#define _tcscmp wcscmp
#define _tcsncmp wcsncmp
#define _putts puts
#define _tcsrchr wcsrchr
#define _tcslen wcslen
#define _tfopen_s(file, mode) fopen(wxFNCONV(file), (char *)mode)
#define _stscanf swscanf
#define _tcscat wcscat
#define _vstprintf(buffer, format, ...) vswprintf(buffer, wcslen(buffer), format, __VA_ARGS__)
#define _vftprintf vfwprintf
#define _tcsnicmp wcsncasecmp
#else
#define _T(z) z
#define TCHAR char
typedef const char *LPCTSTR;
typedef char *LPCTSTR;

#define _tprintf_s _tprintf
#define _tprintf sprintf
#define _tcsicmp strcasecmp
#define _putts puts
#define _tcsrchr strrchr
#define _tcsncpy strncpy
#define _tcscpy_s strcpy
#define _tcscpy strcpy
#define _tcslen strlen
#define _tcsncmp strncmp
#define _tcscmp strcmp
#define _tfopen_s fopen
#define _stscanf sscanf
#define _tcscat strcat
#define _vftprintf vfprintf
#define _vstprintf vsprintf
#define _tcsnicmp strncasecmp
#endif
#endif
#define _strnicmp strncasecmp
#define ARRAYSIZE(z) (sizeof(z)/sizeof((z)[0]))
#define ZeroMemory(dest, size) memset(dest, 0, size)

#elif defined(_MACVER)
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <stddef.h>
#include <time.h>
#include <ctype.h>

/*
typedef char TCHAR;
typedef void *LPVOID;
typedef const char *LPCTSTR;
typedef u_int8_t uint8_t;
typedef u_int16_t uint16_t;
typedef u_int32_t uint32_t;
typedef u_int8_t BYTE, *LPBYTE;
typedef u_int16_t WORD, *LPWORD;
typedef u_int32_t DWORD, *LPDWORD;
 */
/*
#ifndef TRUE
#define FALSE (0)
#define TRUE (!FALSE)
#ifdef WINVER
typedef int BOOL;
#else
typedef signed char BOOL;
#endif
#endif
*/
#define MAX_PATH 256
#define _tprintf_s printf
#define ARRAYSIZE(z) (sizeof(z)/sizeof((z)[0]))
#define _strnicmp strncasecmp
#define _tcsicmp strcasecmp
#define _putts puts
#define _tcsrchr strrchr
#define _tcscpy_s strcpy
#define _tcsncpy strncpy
#define _tcslen strlen
#define _tcscmp strcmp
#define _stscanf sscanf

#endif

#endif
