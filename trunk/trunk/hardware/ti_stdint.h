#ifndef STDINT_H
#define STDINT_H

#include "coretypes.h"

typedef struct STDINT {
	unsigned char intactive;
	double lastchk1;
	double timermax1;
	double lastchk2;
	double timermax2;
	double freq[4];
	int mem;
	int xy;	
	bool on_backup;
	bool on_latch;
} STDINT_t;




#endif
