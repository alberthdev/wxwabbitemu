#ifndef _LABEL_H_TYPES
#define _LABEL_H_TYPES
#include "coretypes.h"
#include <types.h>

typedef struct {
    char *name;
    bool IsRAM;
    uint8_t page;
    uint16_t addr;
} label_struct;

#endif

#ifndef _LABEL_H_PROTOTYPES

#include "calc.h"

#ifdef _HAS_CALC_H
#define _LABEL_H_PROTOTYPES
char* FindAddressLabel(LPCALC lpCalc, waddr_t waddr);
//void ImportBcalls(char* fn);
char* FindBcall(int address);
void FindFlags(int flag, int bit, TCHAR **flagstring, TCHAR **bitstring);

void VoidLabels(LPCALC lpCalc);
label_struct *lookup_label(LPCALC lpCalc, TCHAR *label);
int labels_app_load(LPCALC lpCalc, LPCTSTR lpszFileName);
#endif

#endif

