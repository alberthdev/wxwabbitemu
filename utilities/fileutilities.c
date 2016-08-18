#include "stdafx.h"
#include "coretypes.h"
#ifdef WXVER
#include <wx/wx.h>
extern char* wxStringToChar(wxString);
#endif

#include "fileutilities.h"

int BrowseFile(TCHAR* lpstrFile, const TCHAR *lpstrFilter, const TCHAR *lpstrTitle, const TCHAR *lpstrDefExt, unsigned int flags) {
	lpstrFile[0] = '\0';
#ifdef _WINDOWS
	OPENFILENAME ofn;
	ofn.lStructSize			= sizeof(OPENFILENAME);
	ofn.hwndOwner			= GetForegroundWindow();
	ofn.hInstance			= NULL;
	ofn.lpstrFilter			= (LPCTSTR) lpstrFilter;
	ofn.lpstrCustomFilter	= NULL;
	ofn.nMaxCustFilter		= 0;
	ofn.nFilterIndex		= 0;
	ofn.lpstrFile			= (LPTSTR) lpstrFile;
	ofn.nMaxFile			= MAX_PATH;
	ofn.lpstrFileTitle		= NULL;
	ofn.nMaxFileTitle		= 0;
	ofn.lpstrInitialDir		= NULL;
	ofn.lpstrTitle			= lpstrTitle;
	ofn.Flags				= flags | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_LONGNAMES;
	ofn.lpstrDefExt			= lpstrDefExt;
	ofn.lCustData			= 0;
	ofn.lpfnHook			= NULL;
	ofn.lpTemplateName		= NULL;
	ofn.pvReserved			= NULL;
	ofn.dwReserved			= 0;
	ofn.FlagsEx				= 0;
	if (!GetOpenFileName(&ofn)) {
		return 1;
	}
#elif WXVER
	flags |= wxFD_OPEN | wxFD_FILE_MUST_EXIST;
	wxFileDialog dialog(NULL, lpstrFile, wxEmptyString, lpstrFile, lpstrFilter, flags, wxDefaultPosition, wxDefaultSize, lpstrTitle);
	if (dialog.ShowModal() == wxID_OK) {
		_tcscpy(lpstrFile, dialog.GetPath().c_str());
	} else {
		return 1;
	}
#endif
	return 0;
}

int SaveFile(TCHAR *lpstrFile, const TCHAR *lpstrFilter, const TCHAR *lpstrTitle, const TCHAR *lpstrDefExt, unsigned int flags, unsigned int filterIndex) {
	lpstrFile[0] = '\0';
#ifdef _WINDOWS
	OPENFILENAME ofn;
	ofn.lStructSize			= sizeof(OPENFILENAME);
	ofn.hwndOwner			= GetForegroundWindow();
	ofn.hInstance			= NULL;
	ofn.lpstrFilter			= (LPCTSTR) lpstrFilter;
	ofn.lpstrCustomFilter	= NULL;
	ofn.nMaxCustFilter		= 0;
	ofn.nFilterIndex		= filterIndex;
	ofn.lpstrFile			= (LPTSTR) lpstrFile;
	ofn.nMaxFile			= MAX_PATH;
	ofn.lpstrFileTitle		= NULL;
	ofn.nMaxFileTitle		= 0;
	ofn.lpstrInitialDir		= NULL;
	ofn.lpstrTitle			= lpstrTitle;
	ofn.Flags				= flags | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_LONGNAMES | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt			= lpstrDefExt;
	ofn.lCustData			= 0;
	ofn.lpfnHook			= NULL;
	ofn.lpTemplateName		= NULL;
	ofn.pvReserved			= NULL;
	ofn.dwReserved			= 0;
	ofn.FlagsEx				= 0;
	if (!GetSaveFileName(&ofn)) {
		return 1;
	}
#elif WXVER
	flags |= wxFD_SAVE  | wxFD_OVERWRITE_PROMPT;
	wxFileDialog dialog(NULL, lpstrFile, wxEmptyString, lpstrFile, lpstrFilter, flags, wxDefaultPosition, wxDefaultSize, lpstrTitle);
	dialog.SetFilterIndex(filterIndex);
	if (dialog.ShowModal() == wxID_OK) {
		_tcscpy(lpstrFile, dialog.GetPath().c_str());
	} else {
		return 1;
	}
#endif
	return 0;
}

BOOL ValidPath(TCHAR *lpstrFile) {
	FILE *file;
#ifdef WINVER
	errno_t error = _tfopen_s(&file, lpstrFile, _T("r"));
	if (file)
		fclose(file);
	return error == 0;
#else
	file = _tfopen_s(lpstrFile, "r");
	BOOL error = file == NULL;
	fclose(file);
	return error;
#endif
}

void GetAppDataString(TCHAR *buffer, int len) {
#ifdef WINVER
	TCHAR *env;
	size_t envLen;
	_tdupenv_s(&env, &envLen, _T("appdata"));
	if (!env) {
		return;
	}
	StringCbCopy(buffer, len, env);
	StringCbCat(buffer, len, _T("\\Wabbitemu\\"));
	free(env);
#else
#ifdef _UNICODE
	wxString envString(getenv("appdata"), wxConvUTF8);
	_tcscpy(buffer, envString.c_str());
#else
	_tcscpy(buffer, getenv("appdata"));
#endif
	_tcscat(buffer, _T("/wabbitemu/"));
#endif
}
