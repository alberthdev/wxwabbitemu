#include "guiopenfile.h"
#include "calc.h"
#include "sendfiles.h"

extern char* wxStringToChar(wxString);

void GetOpenSendFileName(wxWindow *frame, int defFilter) {
	int slot =  0;//calc_from_frame(frame);
	int result;
	char lpstrFilter[] 	= "\
Known File Types|*.73p;*.82*;*.83p*;*.8xp*;*.8xk;*.73k;*.sav;*.rom;*.lab;*.8xu|\
Calculator Program Files  (*.73p;*.82*;*.83p*;*.8xp*)|*.73p;*.82*;*.83p*;*.8xp*|\
Calculator Applications  (*.8xk, *.73k)|*.8xk;*.73k|\
Calculator OSes (*.8xu)|*.8xu|\
Save States  (*.sav)|*.sav|\
ROMS  (*.rom)|*.rom|\
Label Files (*.lab)|*.lab|\
All Files (*.*)|*.*\0";
	char filepath[PATH_MAX+256];
	char filestr[PATH_MAX+256];
	char * FileNames = NULL;
	char * filename;
	char * filestroffset;
		
	filepath[0] = '\0';
	filestr[0] = '\0';
	
	wxFileDialog dialog(calcs[slot].wxLCD->frameLCD, wxT("Wabbitemu Open File"),
	wxT(""), wxT(""), wxT(lpstrFilter), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);//, wxDefaultPosition,wxDefaultSize, "filedlg")
	if (dialog.ShowModal() != wxID_OK)
		return;
	wxArrayString filePaths;
	dialog.GetPaths(filePaths);
	for (int i = 0; i < filePaths.GetCount(); i++) {
		FileNames = AppendName(FileNames, wxStringToChar(filePaths[i]));
	}
		
	NoThreadSend(FileNames, SEND_RAM, slot);
}