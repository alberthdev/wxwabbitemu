#ifndef LABEL_H
#define LABEL_H

#include "coretypes.h"
#include <types.h>
#include <stdint.h>

typedef struct {
    char *name;
    bool IsRAM;
    uint8_t page;
    uint16_t addr;
} label_struct;

char* FindAddressLabel( int slot, bool IsRAM, uint8_t page, uint16_t addr);
//void ImportBcalls(char* fn);
char* FindBcall(int address);
void FindFlags(int flag,int bit, char **flagstring, char **bitstring);
void VoidLabels(int slot);
label_struct *lookup_label(char *label);
int labels_app_load(int slot, char* fn);
#endif

