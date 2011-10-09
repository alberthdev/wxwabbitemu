#ifndef CALC_H
#define CALC_H

#include <wx/frame.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "coretypes.h"

#ifdef WXVER
#include "gui_wx.h"
#include "guilcd_wx.h"
#endif
#include "core.h"
#include "lcd.h"
#include "keys.h"

#include "savestate.h"
#include "label.h"
#include "link.h"
#include <stdint.h>
#include "types.h"

#ifdef WXVER
#include "wx/wx.h"
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

#define MIN_BLOCK_SIZE 16
typedef struct profiler {
	bool running;
	int blockSize;
	int lowAddress;
	int highAddress;
	long long totalTime;
	long data[0x10000 / MIN_BLOCK_SIZE];
} profiler_t;

typedef struct tagCALC {
	int slot;
	char rom_path[256];
	char rom_version[32];
	int model;

	time_t time_error;

	bool active;
	CPU_t cpu;
	memory_context_t mem_c;
	timer_context_t timer_c;

#ifdef WXVER
	MyFrame *wxFrame;
	MyLCD *wxLCD;
	wxBitmap calcSkin;
	wxImage keymap;
	int Scale;
	bool SkinEnabled;
	bool bCutout;
	wxSize SkinSize;
	wxRect LCDRect;
	clock_t sb_refresh;
	bool bCustomSkin;
	char skin_path[256];
	char keymap_path[256];
#else
	pthread_t hdlThread;
#endif
	bool running;
	int speed;
	BYTE breakpoints[0x10000];
	//bool warp;
	label_struct labels[6000];
	profiler_t profiler;

	char labelfn[256];
	applist_t applist;
	apphdr_t *last_transferred_app;

	volatile bool send;
	volatile int CurrentFile;
	volatile int FileCnt;
	volatile int BytesSent;
	volatile int SendSize;

	gif_disp_states gif_disp_state;
} calc_t;
typedef struct tagCALC CALC, *LPCALC;

#ifdef WITH_BACKUPS
typedef struct DEBUG_STATE {
	SAVESTATE_t* save;
	struct DEBUG_STATE *next, *prev;
} debugger_backup;
#endif

#define MAX_CALCS	8
#define MAX_SPEED 100*50

int calc_slot_new(void);
u_int calc_count(void);
int calc_reset(int);
int calc_run_frame(int);
int calc_run_seconds(int, double);
int calc_run_timed(int, time_t);
#ifdef _WINDLL
__declspec(dllexport)
#endif
int calc_run_all(void);
#ifdef WITH_BACKUPS
void do_backup(int);
void restore_backup(int, int);
void init_backups();
void free_backups(int);
void free_backup(debugger_backup *);
#endif

#ifdef _WINDLL
__declspec(dllexport)
#endif
int rom_load(int, char *);
void calc_slot_free(int);
#ifdef WXVER
int calc_from_handle(void *frame);
#endif
#endif

int calc_init_83p(int slot);
int calc_init_84p(int slot);
int calc_init_83pse(int slot);

void calc_erase_certificate(u_char*, int);
#ifdef CALC_C
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL calc_t calcs[MAX_CALCS];
GLOBAL int gslot;
GLOBAL int DebuggerSlot;
#ifdef WITH_BACKUPS
#define MAX_BACKUPS 10
GLOBAL debugger_backup* backups[MAX_CALCS];
GLOBAL int number_backup;
GLOBAL int current_backup_index;
#endif
GLOBAL int frame_counter;
GLOBAL bool exit_save_state;
GLOBAL bool load_files_first;
GLOBAL bool do_backups;
GLOBAL bool show_wizard;
GLOBAL bool sync_cores;

//GLOBAL wxString CalcModelTxt[11];

#define HAS_CALC_H
