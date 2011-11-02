#include "droptarget.h"
#include "SendFile.h"
#include "gui_wx.h"

bool DnDFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
	char * FileNames = NULL;
    size_t nFiles = filenames.GetCount();
    for (int i = 0; i < nFiles; i++ ) {
		char *tempPath = wxStringToChar(filenames[i]);
		SendFile(lpCalc, tempPath, SEND_CUR);
		delete tempPath;
	}

    return true;
}
