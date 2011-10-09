#include "calc.h"
#include "state.h"
#include "var.h"
#include "link.h"
#include "sendfiles.h"




static int SlotSave =-1;
static wxWindow *mainWindow = NULL;

typedef struct SENDFILES {
	char* FileNames;
	int ram;
//	HANDLE hdlSend;
} SENDFILES_t;


int SizeofFileList(char* FileNames) {
	int i;
	if (FileNames == NULL) return 0;
	for(i = 0; FileNames[i]!=0 || FileNames[i+1]!=0; i++);
	return i+2;
}

char* AppendName(char* FileNames, char* fn) {
	size_t length;
	char* pnt;
	int i;
	length = strlen(fn);
	if (FileNames == NULL) {
		FileNames = (char *) malloc(length+2);
		memset(FileNames,0,length+2);
		pnt = FileNames;
	} else {
		for(i = 0; FileNames[i]!=0 || FileNames[i+1]!=0; i++);
		i++;
		FileNames = (char *) realloc(FileNames,i+length+2);
		pnt = FileNames+i;
		memset(pnt,0,length+2);
	}
	strcpy(pnt,fn);
	return FileNames;
}

void SendFile( char* FileName , int ram ) {
	bool is_link_connected = link_connected(SlotSave);
	TIFILE_t *var = importvar(FileName, SlotSave, ram);
	LINK_ERR result;
	if (var != NULL) {
		switch(var->type) {
			case BACKUP_TYPE:
			case VAR_TYPE:
			case FLASH_TYPE:
				calcs[SlotSave].SendSize = var->length;
				calcs[SlotSave].cpu.pio.link->vlink_send = 0;
				result = link_send_var(&calcs[SlotSave].cpu, var, (SEND_FLAG) ram);

				if (var->type == FLASH_TYPE) {
					// Rebuild the applist
					state_build_applist(&calcs[SlotSave].cpu, &calcs[SlotSave].applist);

					u_int i;
					for (i = 0; i < calcs[SlotSave].applist.count; i++) {
						if (strncmp((char *) var->flash->name, calcs[SlotSave].applist.apps[i].name, 8) == 0) {
							calcs[SlotSave].last_transferred_app = &calcs[SlotSave].applist.apps[i];
							break;
						}
					}
				}
				break;
			case ROM_TYPE:
			case SAV_TYPE:
				FreeTiFile(var);
				var = NULL;
				rom_load(SlotSave, FileName);
				//SendMessage(calcs[SlotSave].hwndFrame, WM_USER, 0, 0);
				break;
			case LABEL_TYPE: {
				strcpy(calcs[SlotSave].labelfn,FileName);
				printf("loading label file for slot %d: %s\n", SlotSave, FileName);
				VoidLabels(SlotSave);
				labels_app_load(SlotSave, calcs[SlotSave].labelfn);

				break;
			}
			case SKIP_TYPE:
			case BREAKPOINT_TYPE:
				break;
			default:
				break;
		}
		if (var)
			FreeTiFile(var);
		if (is_link_connected)
			link_connect(&calcs[0].cpu, &calcs[1].cpu);
	} else {
	}
}

static char *current_file_sending;

void SendFiles( char* FileNames , int ram ) {
	int i;
	int modelsave;
	unsigned char* fn = (unsigned char *)  FileNames;
	calcs[SlotSave].send = TRUE;
	calcs[SlotSave].running = FALSE;
	calcs[SlotSave].CurrentFile = 0;
	calcs[SlotSave].FileCnt = 0;
	i=0;
	while(FileNames[i]!=0) {
		for(;FileNames[i]!=0;i++);
		i++;
		calcs[SlotSave].FileCnt++;
	}

	while (fn[0] != 0) {
		modelsave = calcs[SlotSave].model;
		calcs[SlotSave].CurrentFile++;
		current_file_sending = (char *) fn;
		SendFile((char *) fn,ram);
		for(;fn[0]!=0;fn++);
		fn++;
	}
	//if (calcs[SlotSave].model == TI_82 && modelsave == calcs[SlotSave].model) end82send(SlotSave);
	calcs[SlotSave].running = TRUE;
	free(FileNames);
	calcs[SlotSave].send = FALSE;
}

void NoThreadSend(const char* FileNames, int ram, int slot) {
	if (SlotSave == -1) {
		SlotSave = slot;
	} else {
		// error;
	}

	SendFiles((char*) FileNames, ram);
}






