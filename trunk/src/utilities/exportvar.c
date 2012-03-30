#include "stdafx.h"

#include "calc.h"
#include "core.h"
#include "link.h"
#include "exportvar.h"

void intelhex (MFILE*, const unsigned char*, int size, int page = 0, int start_address = 0x4000);

const char fileheader[]= {
	'*','*','T','I','8','3','F','*',0x1A,0x0A,0x00};
const char flashheader[] = {
	'*','*','T','I','F','L','*','*'} ;
const char comment[42] = "File Exported by Wabbitemu.";

TCHAR type_ext[][4] = {
	_T("8xn"),
	_T("8xl"),
	_T("8xm"),
	_T("8xy"),
	_T("8xs"),
	_T("8xp"),
	_T("8xp"),
	_T("8xi"),
	_T("8xd"),
	_T(""),
	_T("8xy"),
	_T("8xy"),
	_T("8xc"),
	_T("8xl"),
	_T(""),
	_T("8xw"),
	_T("8xz"),
	_T("8xt"),
	_T(""),
	_T("8xb"),
	_T("8xk"),
	_T("8xv"),
	_T(""),
	_T("8xg"),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T("8xy"),
	_T(""),
	_T(""),
	_T("")
};

MFILE *mopen(const TCHAR *filename, const TCHAR * mode) {
	MFILE* mf= (MFILE *) malloc(sizeof(MFILE));
	memset(mf, 0, sizeof(MFILE));
	if (filename) {
#ifdef WINVER
		fopen_s(&mf->stream, filename, mode);
#else
		mf->stream = fopen(filename, mode);
#endif
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

int mputc(int c, MFILE* mf) {
	unsigned char *temp;
	if (!mf) return EOF;
	if (mf->stream) {
		int temp = fputc(c,mf->stream);
		if (temp!=EOF) mf->size++;
		return temp;
	} else {
		if (mf->pnt >= mf->size) {
			temp = (unsigned char *) realloc(mf->data,mf->size+1);
			if (!temp) return EOF;
			mf->size++;
			mf->data = temp;
		}
		return mf->data[mf->pnt++] = c;
	}
}

int mprintf(MFILE* mf, const char *format, ...) {
	unsigned char *temp;
	if (!mf) return EOF;
	va_list list;
	va_start(list, format);
	if (mf->stream) {
		int temp = vfprintf(mf->stream,format,list);
		va_end(list);
		return temp;
	} else {
		char buffer[1024];
		int i;
#ifdef WINVER
		vsprintf_s(buffer, format, list);
#else
		vsprintf(buffer, format, list);
#endif
		size_t sz_length = strlen(buffer);
		if (mf->pnt >= mf->size) {
			temp = (unsigned char *) realloc(mf->data, mf->size+sz_length);
			if (!temp) return EOF;
			mf->size += sz_length;
			mf->data = temp;
		}
		for (i = 0; i < (int) sz_length; i++) {
			if (!mputc(buffer[i], mf))
				break;
		}
		va_end(list);
		return i;
	}
}

int msize(MFILE* mf) {
	if (!mf) return EOF;
	return mf->size;
}

		
void AddrOffset(int *page, unsigned int *address, int offset) {
	if (*page) {
		int pi = (offset >= 0) ? 1 : -1;
		*address += offset;
		while (*address >= 0x8000) {
			*address -= PAGE_SIZE;
			*page += pi;
		}
	} else {
		*address += offset;
	}
}
	

//Page 0 is interpreted as ram.
int VarRead(LPCALC lpCalc, int page, unsigned int address) {
	unsigned char mem;
	if (address > 0xFFFF) return -1;
	if (page) {
		if (address >= 0x4000 && address < 0x8000) {
			mem = lpCalc->cpu.mem_c->flash[(PAGE_SIZE * page) + address - PAGE_SIZE];
		} else {
			return -1;
		}
	} else {
		if (address >= 0xC000)
			mem = lpCalc->cpu.mem_c->ram[address - 0xC000];
		else if (address >= 0x8000 && address < 0xC000)
			mem = lpCalc->cpu.mem_c->ram[address - PAGE_SIZE];
		else return -1;
	}
	return mem;
}


MFILE *ExportApp(LPCALC lpCalc, TCHAR *fn, apphdr_t *app) {
	MFILE *outfile;
	unsigned int tempnum;
	int i, data_size = PAGE_SIZE * app->page_count;
	uint8_t *buffer = (uint8_t *) malloc(data_size);
	memset(buffer, 0, data_size);
	uint8_t *temp_point = buffer;
	for (tempnum = app->page; tempnum > app->page - app->page_count; tempnum--) {
		u_char (*dest)[PAGE_SIZE] = (u_char (*)[PAGE_SIZE]) lpCalc->cpu.mem_c->flash;
		memcpy(temp_point, &dest[tempnum], PAGE_SIZE);
		temp_point += PAGE_SIZE;
	}
	outfile = mopen(fn, "wb");
	// Lots of pointless header crap 
	for(i = 0; i < 8; i++) mputc(flashheader[i], outfile);
	//version, major.minor
	mputc(0x01, outfile);
	mputc(0x01, outfile);
	//flags
	mputc(0x01, outfile);
	//object type
	mputc(0x88, outfile);
	//date
	mputc(0x01, outfile);
	mputc(0x01, outfile);
	mputc(0x19, outfile);
	mputc(0x97, outfile);
	//name length...wtf? its always 8
	mputc(0x08, outfile);
	//name
	for (i = 0; i < 8; i++) mputc(app->name[i], outfile);
	//filler
	for (i = 0; i < 23; i++) mputc(0x00, outfile);
	//device
	mputc(0x73, outfile);
	//its an app not an OS/cert/license
	mputc(0x24, outfile);
	//filler
	for (i = 0; i < 24; i++) mputc(0x00, outfile);
	//size of Intel hex
	tempnum =  77 * (data_size >> 5) + app->page_count * 17 + 11;
	int size = data_size & 0x1F;
	if (size) tempnum += (size << 1) + 13;
	mputc(tempnum & 0xFF, outfile);	//little endian
	mputc((tempnum >> 8) & 0xFF, outfile);
	mputc((tempnum >> 16) & 0xFF, outfile);
	mputc(tempnum >> 24, outfile);
	//data
	intelhex(outfile, buffer, data_size);
	mprintf(outfile,":00000001FF");
	//checksum
	//TODO: this is the best checksum code I've ever seen...

	//DONE :D
	return outfile;
}

/*
 * Exports an OS based on the pages copied into buffer
 * Expects buffer to be a multiple of PAGE_SIZE
 * size defines the total size of the buffer
 */
MFILE * ExportOS(TCHAR *lpszFile, unsigned char *buffer, int size) {
	MFILE *file = mopen(lpszFile, _T("wb"));
	int i;
	// Lots of pointless header crap 
	for(i = 0; i < 8; i++) mputc(flashheader[i], file);
	//version, major.minor
	mputc(0x01, file);
	mputc(0x01, file);
	//flags
	mputc(0x01, file);
	//object type
	mputc(0x88, file);
	//date
	mputc(0x01, file);
	mputc(0x01, file);
	mputc(0x19, file);
	mputc(0x97, file);
	//name length...wtf? its always 8
	mputc(0x08, file);
	//name
	char name[9] = "basecode";
	for (i = 0; i < 8; i++) mputc(name[i], file);
	//filler
	for (i = 0; i < 23; i++) mputc(0x00, file);
	//device
	mputc(0x73, file);
	//its an app not an OS/cert/license
	mputc(0x23, file);
	//filler
	for (i = 0; i < 24; i++) mputc(0x00, file);
	//size of Intel hex
	int tempnum =  77 * (size >> 5) + (size / PAGE_SIZE) * 17 + 11;
	size = size & 0x1F;
	if (size) {
		tempnum += (size << 1) + 13;	
	}
	mputc(tempnum & 0xFF, file);	//little endian
	mputc((tempnum >> 8) & 0xFF, file);
	mputc((tempnum >> 16) & 0xFF, file);
	mputc(tempnum >> 24, file);
	*(buffer + 0x56) = 0xFF;
	mprintf(file, _T("\r\n"));
	//page 0 needs to start at 0x0000
	intelhex(file, (const unsigned char *) buffer, PAGE_SIZE, 0, 0x0000);
	if (size - PAGE_SIZE > 0) {
		intelhex(file, (const unsigned char *) buffer + PAGE_SIZE, size - PAGE_SIZE, 1,  0x4000);
	}
	mprintf(file, _T(":00000001FF"));
	//TODO: checksum
	return file;
}

MFILE * ExportRom(TCHAR *lpszFile, LPCALC lpCalc) {
	MFILE *file = mopen(lpszFile, _T("wb"));
	char* rom = (char *) lpCalc->mem_c.flash;
	int size = lpCalc->mem_c.flash_size;
	if (size != 0 && rom != NULL && file !=NULL) {
		u_int i;
		for(i = 0; i < size; i++) {
			mputc(rom[i], file);
		}
	}
	return file;
}


/* Convert binary buffer to Intel hex in TI format
 * All pages are only $4000 bytes long. 
 * stolen from spasm's export.c  to make my 1/2 hour deadline
 */
void intelhex (MFILE* outfile, const unsigned char* buffer, int size, int page, int start_address) {
	const char hexstr[] = "0123456789ABCDEF";
	int bpnt = 0;
	unsigned int address, ci, temp, i;
	unsigned char chksum;
	unsigned char outbuf[128];
	
	//We are in binary mode, we must handle carriage return ourselves.
   
	while (bpnt < size) {
		mprintf(outfile, ":02000002%04X%02X\r\n", page & 0x1F, (unsigned char) ((~(0x04 + page)) + 1));
		page++;
		address = start_address;   
		for (i = 0; bpnt < size && i < 512; i++) {
			 chksum = (address >> 8) + (address & 0xFF);
			 for(ci = 0; (ci < 64) && (bpnt < size); ci++) {
				temp = buffer[bpnt++];
				outbuf[ci++] = hexstr[temp >> 4];
				outbuf[ci] = hexstr[temp & 0x0F];
				chksum += temp;
			}
			outbuf[ci] = 0;
			ci >>= 1;
			mprintf(outfile,":%02X%04X00%s%02X\r\n", ci, address, outbuf, (unsigned char)(~(chksum + ci) + 1));
			address += 0x20;
		}         
	}
}

//Progs, List AppVar and Group
MFILE *ExportVar(LPCALC lpCalc, TCHAR* fn, symbol83P_t* sym) {
	MFILE *outfile;
	unsigned char mem[0x10020];
	int i, b, size;
	int page = sym->page;
	unsigned int a = sym->address;
	unsigned short chksum = 0;
	
	//Technically no variable can be larger than 65536 bytes,
	//to make reading easier I'm gonna copy all the max file size 
	//into mem.
	for(i = 0; i < 0x10020 && (b = VarRead(lpCalc, page, a)) != -1; i++) {
		mem[i] = b;
		AddrOffset(&page, &a, 1);
	}

	a = 0;
	if (sym->page) {
		if (sym->type_ID == ListObj		|| 
			sym->type_ID == ProgObj 	||
			sym->type_ID == ProtProgObj ||
			sym->type_ID == AppVarObj	||
			sym->type_ID == GroupObj ) {
			a += 3 + 6;
			b = mem[a];
			a += b + 1;
		} else {
			a += 9 + 3;
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
			size = mem[a] + (mem[a + 1] * 256);
			size = (size * 9) + 2;
			break;
		case CListObj:
			size = mem[a] + (mem[a + 1] * 256);
			size = (size * 18) + 2;
			break;
		case MatObj:
			size = mem[a]*mem[a + 1];
			size = (size * 9) + 2;
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
			size = mem[a] + (mem[a + 1] << 8) + 2;
			break;
		default:
			size = mem[a] + (mem[a + 1] << 8) + 2;
			printf("Unknown obj: %02X\n", sym->type_ID);
			break;
	}
		
	outfile = mopen(fn,"wb");

	// Lots of pointless header crap 
	for(i = 0; i < 11; i++) mputc(fileheader[i],outfile);
	for(i = 0; i < 42; i++) mputc(comment[i],outfile);
	mputc((size+17) &  0xFF, outfile);
	mputc((size+17) >> 0x08, outfile);
	

	chksum  = mputc(0x0D, outfile);
	chksum += mputc(0x00, outfile);

	chksum += mputc(size & 0xFF, outfile);
	chksum += mputc(size >> 8, outfile);
	chksum += mputc(sym->type_ID, outfile);
	
	for(i = 0; i < 8 && sym->name[i]; i++) chksum += mputc(sym->name[i], outfile);
	for(;i < 8; i++) mputc(0, outfile);


	chksum += mputc(0x00, outfile); // sym->Resevered[1]

	if (sym->page)
		chksum += mputc(0x80, outfile); // archived
	else
		chksum += mputc(0x00, outfile);

	chksum += mputc(size & 0xFF, outfile);
	chksum += mputc(size >> 8, outfile);

	// Actual program data!
	for(i = 0; i < size; i++) {
		chksum += mputc(mem[a++], outfile);
	}

	mputc(chksum & 0xFF, outfile);
	mputc((chksum >> 8) & 0xFF, outfile);
	return outfile;
}
