/* Super minimal header file that controls all. */

#ifndef _STDAFX_H
#define _STDAFX_H

#if defined(_LINUX) || defined(_MACVER)
#include <win2posix.h>
#endif

#define ARRAYSIZE(z) (sizeof(z)/sizeof((z)[0]))
#define ZeroMemory(dest, size) memset(dest, 0, size)
#define NumElm(array) (sizeof (array) / sizeof ((array)[0]))

#endif
