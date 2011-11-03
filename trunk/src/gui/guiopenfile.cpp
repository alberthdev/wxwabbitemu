#include "guiopenfile.h"
#include "calc.h"
#include "SendFile.h"

void GetOpenSendFileName(LPCALC lpCalc, int defFilter) {
	int result;
	wxString lpstrFilter 	= wxT("\
Known File Types|*.73p;*.73P;*.82*;*.83p*;*.83P*;*.8xp*;*.8Xp*;*.8XP*;*.8xP*;*.8xk;*.8Xk;*.8XK;*.8xK;*.73k;*.73K;*.sav;*.rom;*.lab;*.8xu;*.8Xu;*.8xU;*.8XU|\
Calculator Program Files  (*.73p;*.82*;*.83p*;*.8xp*)|*.73p;*.73P;*.82*;*.83p*;*.83P*;*.8xp*;*.8Xp*;*.8XP*;*.8xP*|\
Calculator Applications  (*.8xk, *.73k)|*.8xk;*.8Xk;*.8XK;*.8xK;*.73k;*.73K|\
Calculator OSes (*.8xu)|*.8xu;*.8Xu;*.8xU;*.8XU|\
Save States  (*.sav)|*.sav|\
ROMS  (*.rom)|*.rom|\
Label Files (*.lab)|*.lab|\
All Files (*.*)|*.*\0");
	char filepath[PATH_MAX+256];
	char filestr[PATH_MAX+256];
	char * FileNames = NULL;
	char * filename;
	char * filestroffset;
		
	filepath[0] = '\0';
	filestr[0] = '\0';
	
	wxFileDialog dialog(NULL, wxT("Wabbitemu Open File"),
	wxT(""), wxT(""), lpstrFilter, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);//, wxDefaultPosition,wxDefaultSize, "filedlg")
	if (dialog.ShowModal() != wxID_OK)
		return;	
	wxArrayString filePaths;
	dialog.GetPaths(filePaths);
	for (int i = 0; i < filePaths.GetCount(); i++) {
		char *tempPath = wxStringToChar(filePaths[i]);
		SendFile(lpCalc, tempPath, SEND_CUR);
		delete tempPath;
	}
}
