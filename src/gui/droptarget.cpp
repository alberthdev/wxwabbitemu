#include "droptarget.h"
#include "sendfile.h"
#include "gui_wx.h"

bool DnDFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
    size_t nFiles = filenames.GetCount();
    for (int i = 0; i < nFiles; i++ ) {
		char *tempPath = wxStringToChar(filenames[i]);
		SendFile(lpCalc, tempPath, SEND_CUR);
		delete tempPath;
	}

    return true;
}
