#define LABEL_C
#include "label.h"
#include "core.h"
#include "calc.h"

#include "bcalls.h"
#include "flags.h"

label_struct *lookup_label(LPCALC lpCalc, char *label_name) {
	int i;
	for (i = 0; lpCalc->labels[i].name != NULL; i++) {
		if (strcmp(lpCalc->labels[i].name, label_name) == 0)
			return &lpCalc->labels[i];
	}
	return NULL;
}	
	

void VoidLabels(LPCALC lpCalc) {
	int i;
	
	for (i = 0; lpCalc->labels[i].name != NULL; i++) {
		free(lpCalc->labels[i].name);
		lpCalc->labels[i].name = NULL;
	}
}

char* FindAddressLabel(LPCALC lpCalc, waddr_t waddr) {
	int i;
	
	for (i = 0; lpCalc->labels[i].name != NULL; i++) {
		label_struct *label = &lpCalc->labels[i];
		if (label->IsRAM == waddr.is_ram && label->page == waddr.page && label->addr == waddr.addr)
			return label->name;
	}
	return NULL;
}
	
//-------------------------------------------
// True means label is found and is the same
//
bool label_search_tios(char *label,int equate) {
	int i,b;
	
	if (!label) return FALSE;

	for(i=0;bcalls[i].address != -1; i++ ) {
		if (strcasecmp(label, bcalls[i].name) == 0) {
			if (bcalls[i].address == (equate&0xFFFF) ) {
				return TRUE;
			}
		}
	}
	
	for(i=0; flags83p[i].flag != -1; i++ ) {
		if (strcasecmp(label, flags83p[i].name) == 0) {
			if (flags83p[i].flag == (equate&0xFFFF)) {
				return TRUE;
			}
		}
		for(b=0;b<8;b++) {
			if (strcasecmp(label, flags83p[i].bits[b].name) == 0) {
				if (flags83p[i].bits[b].bit == (equate&0xFFFF)) {
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
	

int labels_app_load(LPCALC lpCalc, LPCTSTR lpszFileName) {
	FILE *labelFile = NULL;
	int i, length;
#ifdef _UNICODE
	char readBuf[256];
#endif
	char buffer[256];
	char name[256];
#ifdef _WINDOWS
	TCHAR *fileName = ((TCHAR *) lpszFileName) + _tcslen(lpszFileName);
#else
	char *fileName = ((char *) lpszFileName) + strlen(lpszFileName);
#endif
	while (*--fileName != '\\');
	fileName++;

	unsigned int equate;
	label_struct *label = &lpCalc->labels[0];	

#ifdef _WINDOWS
	_tfopen_s(&labelFile, lpszFileName, _T("r"));
#else
	labelFile = fopen(lpszFileName, "r");
#endif
	if (labelFile == NULL) {
		puts("Error opening label files.");
		return 1;
	}
	
	// Clear out the old labels
	VoidLabels(lpCalc);

	while (!feof(labelFile)) {
#ifdef _UNICODE
		fgets(readBuf, 256, labelFile);
/* Temp fix to Unicode frenzy */
#ifdef _WINDOWS
		MultiByteToWideChar(CP_ACP, 0, readBuf, -1, buffer, ARRAYSIZE(buffer));
#else
		fgets(buffer, 256, labelFile);
#endif

#else
#ifdef _WINDOWS
		_fgetts(buffer, 256, labelFile);
#else
		fgets(buffer, 256, labelFile);
#endif
#endif
		i = 0;
		if (buffer[0] != ';')
#ifdef _WINDOWS
			i = _stscanf(buffer, _T("%s = $%X"), name, &equate);
#else
			i = sscanf(buffer, "%s = $%X", name, &equate);
#endif
		if (i == 2) {
			length = (int) strlen(name);
			if (!label_search_tios(name, equate)) {
				
				label->name = (TCHAR *) malloc((length + 1) * sizeof(TCHAR));
#ifdef _WINDOWS
				StringCchCopy(label->name, length + 1, name);
#else
				strcpy(label->name, name);
#endif

				label->addr = equate & 0xFFFF;

				if ( (equate & 0x0000FFFF) >= 0x4000 && (equate & 0x0000FFFF) < 0x8000) {
					int page_offset = (equate >> 16) & 0xFF;
					
					label->IsRAM = FALSE;
					if (lpCalc->last_transferred_app == NULL) {
						upages_t upage;
						state_userpages(&lpCalc->cpu, &upage);
						label->page = upage.start;
					} else {
						applist_t applist;
						state_build_applist(&lpCalc->cpu, &applist);
						for (int i = 0; i < applist.count; i++) {
							int len = 8;
							TCHAR *ptr = applist.apps[i].name + len - 1;
							while (isspace(*ptr--))
								len--;
							if (!strnicmp(fileName, applist.apps[i].name, len)) {
								label->page = applist.apps[i].page;
								break;
							}
						}
					}
				} else {
					label->IsRAM = TRUE;
					label->page = 1;
				}
				label++;
			}
		}
	}
	fclose(labelFile);
	return 0;
}
	
/*
void ImportBcalls(char* fn) {
	int i,address;
	char string[256],tmp[32];
	FILE* infile;
	
	infile = fopen(fn,"r");
	
	if (!infile) {
		puts("COuld not open bcall file");
		return;
	}
	for(address=0;address<65536;address++) {
		for(i=0;i<32;i++) bcalls[address][i] = 0;
	}		
	while( !feof(infile) ) {
		fgets(string,256,infile);
		i = sscanf(string,"%s = $%04X",tmp,&address);
		if (i == 2) {
			strcpy(bcalls[address],tmp);
		}
	}
	fclose(infile);
}
*/
char* FindBcall(int address) {
	int i;
	for(i=0;bcalls[i].address != -1; i++ ) {
		if (bcalls[i].address == address) {
			return bcalls[i].name;
		}
	}
	return NULL;
}


void FindFlags(int flag,int bit, char **flagstring, char **bitstring) {
	int i,b;
	for(i=0; flags83p[i].flag != -1; i++ ) {
		if (flags83p[i].flag == flag) {
			for(b=0;b<8;b++) {
				if (flags83p[i].bits[b].bit == bit) {
					*flagstring = flags83p[i].name;
					*bitstring  = flags83p[i].bits[b].name;
					return;
				}
			}
		}
	}
	*flagstring = NULL;
	*bitstring  = NULL;
}
	
	
	
	
	

