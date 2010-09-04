#ifndef CALC_H
#define CALC_H

#ifdef WINVER
#define MAX_OLE_BREAKPOINTS	128
#endif

#include "coretypes.h"

#ifdef WINVER
#include "gui.h" // it would be nice to get this separated somehow
#include "wbded.h"
#endif
#include "core.h"
#include "lcd.h"
#include "keys.h"
#ifdef WINVER
#include "sound.h" // FIXME: sound is nice
#endif
#include "savestate.h"
#include "label.h"
#include "link.h"
#ifndef WINVER
#include "types.h"
#endif

#ifdef QTVER
#include <pthread.h>
#endif


typedef enum {
	GDS_IDLE,
	GDS_STARTING,
	GDS_RECORDING,
	GDS_ENDING
} gif_disp_states;

typedef struct calc {
	int slot;
	char rom_path[256];
	char rom_version[32];
	int model;

	time_t time_error;
	
	BOOL active;
	CPU_t cpu;
	memory_context_t mem_c;
	timer_context_t timer_c;
#ifdef WINVER
	AUDIO_t* audio; // FIXME: Bad!
#endif
#ifdef WINVER
	WB_IDropTarget *pDropTarget;
	HWND hwndFrame;
	HWND hwndLCD;
	HWND hwndStatusBar;
	HWND hwndDebug;
	
	BOOL SkinEnabled;
	DWORD Scale;
	BOOL bCutout;
	HANDLE hdlThread;
	clock_t sb_refresh;
	
	HWND ole_callback;
	BOOL do_drag;
	HDC hdcSkin;
	HDC hdcKeymap;
#elif WXVER
	cxFrame cxFrame;
#else
	pthread_t hdlThread;
#endif
	gif_disp_states gif_disp_state;
	BOOL running;
	BYTE breakpoints[0x10000];
	BOOL warp;
	label_struct labels[6000];
	
	char labelfn[256];
	applist_t applist;
	apphdr_t *last_transferred_app;

	volatile BOOL send;
	volatile int CurrentFile;
	volatile int FileCnt;
	volatile int BytesSent;
	volatile int SendSize;

} calc_t;


#define MAX_CALCS	8

int calc_slot_new(void);
u_int calc_count(void);
int calc_reset(int);
int calc_run_frame(int);
int calc_run_seconds(int, double);
int calc_run_timed(int, time_t);
int calc_run_all(void);
int rom_load(int, char *);
void calc_free(int);
#ifdef WINVER
int calc_from_hwnd(HWND);
#endif
#endif

#ifdef CALC_C
#define GLOBAL
#else
#define GLOBAL extern
#endif


GLOBAL calc_t calcs[MAX_CALCS];
GLOBAL int gslot;

GLOBAL const char CalcModelTxt[][16]
#ifdef CALC_C
= {	"???",
	"TI-82",
	"TI-83",
	"TI-85",
	"TI-86",
	"TI-73",
	"TI-83+",
	"TI-83+SE",
	"TI-84+",
	"TI-84+SE",
	"???"}
#endif
;


#define HAS_CALC_H
