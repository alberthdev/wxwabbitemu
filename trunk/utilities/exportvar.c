#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#include "core.h"
#include "link.h"
#include "exportvar.h"


const char fileheader[]= {
    '*','*','T','I','8','3','F','*',0x1A,0x0A,0x00};

const char comment[42]="File Exported by Wabbitemu.";

unsigned char type_ext[][4] = {
	"8xn",
	"8xl",
	"8xm",
	"8xy",
	"8xs",
	"8xp",
	"8xp",
	"8xi",
	"8xd",
	"",
	"8xy",
	"8xy",
	"8xc",
	"8xl",
	"",
	"8xw",
	"8xz",
	"8xt",
	"",
	"8xb",
	"8xk",
	"8xv",
	"",
	"8xg"
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"8xy",
	"",
	"",
	""
};




MFILE *mopen(const char *filename,const char * mode ) {
	MFILE* mf= malloc(sizeof(MFILE));
	memset(mf,0,sizeof(MFILE));
	if (filename) {
		mf->stream = fopen(filename,mode);
		if (!mf->stream) {
			free(mf);
			return NULL;
		}
	}
	return mf;
}

int mclose(MFILE* mf) {
	if (!mf) return EOF;
	if (mf->stream) {
		fclose(mf->stream);
	}
	free(mf->data);
	free(mf->name);
	free(mf);
	return 0;
}
int meof(MFILE* mf) {
	if (!mf) return EOF;
	if (mf->stream) {
		return feof(mf->stream);
	} else {
		return mf->eof;
	}
}

int mgetc(MFILE* mf) {
	if (!mf) return EOF;
	if (mf->stream) {
		return fgetc(mf->stream);
	} else {
		if (mf->pnt>=mf->size) {
			mf->eof = EOF;
			return EOF;
		} else {
			return mf->data[mf->pnt++];
		}
	}
}

int mputc(int c,MFILE* mf) {
	unsigned char *temp;
	if (!mf) return EOF;
	if (mf->stream) {
		int temp = fputc(c,mf->stream);
		if (temp!=EOF) mf->size++;
		return temp;
	} else {
		if (mf->pnt >= mf->size) {
			temp = realloc(mf->data,mf->size+1);
			if (!temp) return EOF;
			mf->size++;
			mf->data = temp;
		}
		return mf->data[mf->pnt++] = c;
	}
}

int msize(MFILE* mf) {
	if (!mf) return EOF;
/*
	if (mf->stream) {
		long int curpos = ftell(mf->stream);
		fseek(mf->stream,0,SEEK_END);
		long int size = ftell(mf->stream);
		fseek(mf->stream,curpos,SEEK_SET);
	}
	*/
	return mf->size;
}

		
void AddrOffset(int *page,unsigned int *address,int offset) {
	if (*page) {
		int pi = (offset>=0)?1:-1;
		*address += offset;
		while (*address >= 0x8000) {
			*address-=0x4000;
			*page+=pi;
		}
	} else {
		*address += offset;
	}
}
	

//Page 0 is interpreted as ram.
int VarRead(int slot, int page, unsigned int address) {
	unsigned char mem;
	if (address > 0xFFFF) return -1;
	if (page) {
		if (address>=0x4000 && address<0x8000) {
			mem = calcs[slot].mem_c.flash[(16384*page)+address-0x4000];
		} else {
			return -1;
		}
	} else {
		if (address>=0xC000) {
			mem = calcs[slot].mem_c.ram[address-0xC000];
		} else if (address>=0x8000 && address<0xC000) {
			mem = calcs[slot].mem_c.ram[address-0x4000];
		} else {
			return -1;
		}
	}
	return mem;
}
	
//Prog�s, List AppVar and Group

MFILE *ExportVar(int slot, char* fn, symbol83P_t* sym) {
	MFILE *outfile;
	unsigned char mem[0x10020];
    int i,b,size;
    int page = sym->page;
    unsigned int a = sym->address;
    unsigned short chksum = 0;
    
    //Technically no variable can be larger than 65536 bytes,
    //to make reading easier I'm gonna copy all the max file size 
    //into mem.
    for(i=0;i<0x10020 && (b=VarRead(slot,page,a))!=-1;i++) {
		mem[i] =b;
		AddrOffset(&page,&a,1);
	}   

    a = 0;
    if (sym->page) {
		if (	sym->type_ID == ListObj		|| 
				sym->type_ID == ProgObj 	||
				sym->type_ID == ProtProgObj || 
				sym->type_ID == GroupObj 	) {
			a += 3+6;
			b =mem[a];
			a += b+1;
		} else {
			a += 9+3;
		}
	
	}

	
	switch(sym->type_ID) {
		case RealObj:
			size = 9;
			break;
		case CplxObj:
			size = 18;
			break;
		case ListObj:
			size = mem[a]+(mem[a+1]*256);
			size = (size*9)+2;
			break;
		case CListObj:
			size = mem[a]+(mem[a+1]*256);
			size = (size*18)+2;
			break;
		case MatObj:
			size = mem[a]*mem[a+1];
			size = (size*9)+2;
			break;
		case ProgObj:
		case ProtProgObj:
		case PictObj:
		case EquObj:
		case NewEquObj:
		case StrngObj:
		case GDBObj:
		case AppVarObj:
		case GroupObj:
			size = mem[a]+(mem[a+1]<<8)+2;
			break;
		default:
			size = mem[a]+(mem[a+1]<<8)+2;
			printf("Unknown obj: %02X\n",sym->type_ID);
			break;
	}
    	
    outfile = mopen(fn,"wb");

    // Lots of pointless header crap 
    for(i = 0; i < 11; i++) mputc(fileheader[i],outfile);
    for(i = 0; i < 42; i++) mputc(comment[i],outfile);
    mputc((size+17) &  0xFF,outfile);
    mputc((size+17) >> 0x08,outfile);
    

    chksum  = mputc(0x0D,outfile);
    chksum += mputc(0x00,outfile);

    chksum += mputc(size&0xFF,outfile);
    chksum += mputc(size>>8,outfile);
    chksum += mputc(sym->type_ID,outfile);
    
    for(i = 0; i < 8 && sym->name[i]; i++) chksum += mputc(sym->name[i], outfile);
    for(;i < 8; i++) mputc(0,outfile);


    chksum += mputc(0x00,outfile); // sym->Resevered[1]

	if (sym->page) {
	    chksum += mputc(0x80,outfile); // archived
	} else {
		chksum += mputc(0x00,outfile);
	}


    chksum += mputc(size & 0xFF,outfile);
    chksum += mputc(size>>8,outfile);

    // Actual program data!
    for(i = 0; i < size; i++) {
        chksum += mputc(mem[a++],outfile);
    }

    mputc(chksum & 0xFF,outfile);
    mputc((chksum >> 8) & 0xFF,outfile);
	return outfile;
}


