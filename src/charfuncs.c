#include <stdio.h>
#include "stdafx.h"

// Special stringy functions
int putst (const wchar_t * str) {
	char *charstr;
	wcstombs(charstr, str, sizeof(str));
	puts(charstr);
}
int _wcsicmp(const char* cs,const wchar_t * ct)  
{
	while (towlower(*cs) == towlower(*ct))
  	{
    		if (*cs == 0)
      			return 0;
    		cs++;
    		ct++;
  	}
  	return towlower(*cs) - towlower(*ct);
	
}
