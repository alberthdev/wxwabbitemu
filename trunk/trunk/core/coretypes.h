#ifndef CORETYPES_H_
#define CORETYPES_H_

#define NumElm(array) (sizeof (array) / sizeof ((array)[0]))

#include <stdint.h>

#ifdef WINVER
#include <winsock.h>
#else
#include <sys/types.h>
#endif


#ifndef BOOL
typedef int BOOL;
#endif
#ifndef FALSE
#define FALSE (0)
#define TRUE (!FALSE)
#endif

#include <stdio.h>

#endif /*CORETYPES_H_*/
