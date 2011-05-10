#include "droptarget.h"
#include "sendfiles.h"
#include "calc.h"

bool DnDFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
	char * FileNames = NULL;
    size_t nFiles = filenames.GetCount();
    for (int i = 0; i < nFiles; i++ )
		FileNames = AppendName(FileNames, wxStringToChar(filenames[i]));
	
	NoThreadSend(FileNames, SEND_RAM, gslot);

    return true;
}