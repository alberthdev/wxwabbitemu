#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "lcd.h"

typedef struct {
	char tag[4];
	int pnt;
	int size;
	unsigned char *data;
} CHUNK_t;


typedef struct {
	int version_major;
	int version_minor;
	int version_build;
	int model;
	int chunk_count;
	TCHAR author[32];
	TCHAR comment[64];	
	CHUNK_t* chunks[512];
} SAVESTATE_t;


#define CUR_MAJOR 0
#define CUR_MINOR 1
#define CUR_BUILD 1


#define DETECT_STR		"*WABBIT*"
#define DETECT_CMP_STR	"*WABCMP*"
#define FLASH_HEADER	"**TIFL**"

#define NO_CMP			0
#define ZLIB_CMP		1

#define SAVE_HEADERSIZE	116

#define INFO_tag				"INFO"
#define CPU_tag					"CPU "
#define MEM_tag					"MEMC"
#define ROM_tag					"ROM "
#define RAM_tag					"RAM "
#define TIMER_tag				"TIME"
#define LCD_tag					"LCD "
#define LINK_tag				"LINK"
#define STDINT_tag				"STDI"
#define SE_AUX_tag				"SEAX"
#define USB_tag					"USB "
#define REMAP_tag				"RMAP"
#define RAM_LIMIT_tag			"RMLM"
#define RAM_BREAKS_tag			"RBRK"
#define FLASH_BREAKS_tag		"FBRK"
#define NUM_FLASH_BREAKS_tag	"NFBK"
#define NUM_RAM_BREAKS_tag		"NRBK"

void WriteSave(const TCHAR *, SAVESTATE_t *, int);
void LoadSlot(SAVESTATE_t* , void *lpInput);
SAVESTATE_t* SaveSlot(void *lpInput);
SAVESTATE_t* CreateSave(const TCHAR *, const TCHAR *, int);
SAVESTATE_t* ReadSave(FILE *ifile);
void FreeSave(SAVESTATE_t *);
char* GetRomOnly(SAVESTATE_t *save, int *);
void LoadLCD(SAVESTATE_t *, LCD_t *);

#endif

