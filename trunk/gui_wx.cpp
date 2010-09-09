#include "gui_wx.h"
#include "calc.h"

bool gif_anim_advance;
bool silent_mode = false;
enum
{
	ID_File_New,
	ID_File_Open,
	ID_File_Save,
	ID_File_Gif,
	ID_File_Close,
	ID_File_Quit,

	ID_Edit_Copy,
	ID_Edit_Paste,
	
	ID_Calc_Skin,
	ID_Calc_Sound,
	ID_Calc_Pause,
	ID_Calc_Connect,
	ID_Calc_Vars,
	ID_Calc_Options,
	
	ID_Debug_Reset,
	ID_Debug_Open,
	
    ID_Help_About,
	ID_Help_Website
};

/*BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_SHOW(MyFrame::OnShow)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
END_EVENT_TABLE()*/

class MyApp: public wxApp
{
    virtual bool OnInit();
	//virtual int MainLoop();
	void OnTimer(wxTimerEvent& event);
	wxTimer *timer;
};

IMPLEMENT_APP(MyApp)
char* wxStringToChar(wxString input)
{
#if (wxUSE_UNICODE)
        size_t size = input.size() + 1;
        char *buffer = new char[size];//No need to multiply by 4, converting to 1 byte char only.
        memset(buffer, 0, size); //Good Practice, Can use buffer[0] = '&#65533;' also.
        wxEncodingConverter wxec;
        wxec.Init(wxFONTENCODING_ISO8859_1, wxFONTENCODING_ISO8859_1, wxCONVERT_SUBSTITUTE);
        wxec.Convert(input.mb_str(), buffer);
        return buffer; //To free this buffer memory is user responsibility.
#else
        return (char *)(input.c_str());
#endif
}

char load_file_buffer[PATH_MAX];
bool LoadRomIntialDialog() {
	char *strFilter 	= "\
Known types ( *.sav; *.rom)|*.sav;*.rom|\
Save States  (*.sav)|*.sav|\
ROMs  (*.rom)|*.rom| \
All Files (*.*)|*.*";
	wxFileDialog dialog(NULL, wxT("Wabbitemu: Please select a ROM or save state"),
	wxT(""), wxT(""), wxT(strFilter), wxFD_OPEN | wxFD_FILE_MUST_EXIST);//, wxDefaultPosition,wxDefaultSize, "filedlg")
	if (dialog.ShowModal() == wxID_OK) {
		strcpy(load_file_buffer, wxStringToChar(dialog.GetPath()));
		return true;
	}
	else
		return false;
}

bool MyApp::OnInit()
{
	int slot = calc_slot_new();
	calcs[slot].SkinEnabled = false;
	slot = rom_load(slot, calcs[gslot].rom_path);
	if (slot != -1) gui_frame(slot);
	else {
		bool loadFile = LoadRomIntialDialog();
		//wxMessageBox(string, wxT("OnInit"), wxOK, NULL);
		//printf("hello", string);
		if (loadFile) {
			slot = calc_slot_new();
			slot = rom_load(slot, load_file_buffer);
			if (slot != -1) gui_frame(slot);
			else return FALSE;
		} else return FALSE;
	}
	timer = new wxTimer();
	timer->Connect(wxEVT_TIMER, (wxObjectEventFunction) &MyApp::OnTimer);
	timer->Start(FPS);
    return TRUE;
}

/*int MyApp::MainLoop() {
	
}*/

void MyApp::OnTimer(wxTimerEvent& event) {
	static int difference;
	static struct timeval prevTimer;
	struct timeval time;
	gettimeofday(&time, NULL);
	

	// How different the timer is from where it should be
	// guard from erroneous timer calls with an upper bound
	// that's the limit of time it will take before the
	// calc gives up and claims it lost time
	difference += ((time.tv_sec - prevTimer.tv_sec) & 0x003F) - TPF;
	prevTimer = time;
	//this is for updating the progress bar
	//we will use this later
	/*int i;
	for (i = 0; i < MAX_CALCS; i++) {
		if (calcs[i].active && calcs[i].send == TRUE) {
			static int frameskip = 0;
			frameskip = (frameskip + 1) % 3;

			if (frameskip == 0) {
				extern HWND hwndSend;
				SendMessage(hwndSend, WM_USER, 0, 0);
				difference = 0;
				return;
			}
		}
	}*/

	// Are we greater than Ticks Per Frame that would call for
	// a frame skip?
	if (difference > -TPF) {
		calc_run_all();
		while (difference >= TPF) {
			calc_run_all();
			difference -= TPF;
		}

		int i;
		for (i = 0; i < MAX_CALCS; i++) {
			if (calcs[i].active) {
				gui_draw(i);
			}
		}
	// Frame skip if we're too far ahead.
	} else difference += TPF;
}


int gui_draw(int slot) {
	gslot = slot;
	calcs[slot].wxLCD->Refresh();
	//UpdateWindow(calcs[slot].hwndLCD);

	//UpdateDebugTrack();
	if (calcs[slot].gif_disp_state != GDS_IDLE) {
		static int skip = 0;
		if (skip == 0) {
			gif_anim_advance = true;
			calcs[slot].wxFrame->Update();
		}
		skip = (skip + 1) % 4;
	}
}

int gui_frame(int slot) {
	if (!calcs[slot].Scale)
		calcs[slot].Scale = 2;
	// Set gslot so the CreateWindow functions operate on the correct calc
	gslot = slot;
	
	calcs[slot].wxFrame = new MyFrame(slot);
	calcs[slot].wxFrame->frameMain->Show(true);
	
	calcs[slot].wxLCD = new MyLCD();
	calcs[slot].wxLCD->frameLCD->Show(true);
	if (calcs[slot].wxFrame == NULL /*|| calcs[slot].hwndLCD == NULL*/) return -1;
	calcs[slot].running = TRUE;
	calcs[slot].speed = 100;
	wxMenuBar *wxMenu = calcs[slot].wxFrame->frameMain->GetMenuBar();
	//CheckMenuRadioItem(GetSubMenu(GetSubMenu(hmenu, 2),4), IDM_SPEED_QUARTER, IDM_SPEED_MAX, IDM_SPEED_NORMAL, MF_BYCOMMAND);
	gui_frame_update(slot);
	return 0;
}

int gui_frame_update(int slot) {
	wxMenuBar *wxMenu = calcs[slot].wxFrame->frameMain->GetMenuBar();
	if (wxMenu != NULL) {
		if (!calcs[slot].SkinEnabled) {
			wxMenu->Check(ID_Calc_Skin, false);
			// Create status bar
			wxStatusBar *wxStatus = calcs[slot].wxFrame->frameMain->GetStatusBar();
			if (wxStatus == NULL)
				wxStatus = calcs[slot].wxFrame->frameMain->CreateStatusBar(1, wxST_SIZEGRIP, wxID_ANY );
			int iStatusWidths[] = {100, -1};
			wxStatus->SetStatusWidths(2, iStatusWidths);
			wxStatus->SetStatusText(wxT(CalcModelTxt[calcs[slot].model]), 0);
			//wxMessageBox(CalcModelTxt[calcs[slot].model], wxT("OnInit"), wxOK, NULL);
			wxSize skinSize(128*calcs[slot].Scale, 64*calcs[slot].Scale);
			if (wxMenu)
				skinSize.IncBy(0, wxSystemSettings::GetMetric(wxSYS_MENU_Y, calcs[slot].wxFrame->frameMain));
			calcs[slot].wxFrame->frameMain->SetClientSize(skinSize);
		} else {
			wxMenu->Check(ID_Calc_Skin, true);
			wxStatusBar *wxStatus = calcs[slot].wxFrame->frameMain->GetStatusBar();
			if (wxStatus != NULL) {
				wxStatus->Destroy();
			}
			wxSize skinSize(350, 725);
			if (wxMenu)
				skinSize.IncBy(0, wxSystemSettings::GetMetric(wxSYS_MENU_Y, calcs[slot].wxFrame->frameMain));
			calcs[slot].wxFrame->frameMain->SetClientSize(skinSize);
		}
	}
}

int gui_debug(int slot) {
	return 0;
}

MyFrame::MyFrame(int slot) {
	wxSize windowSize;
	frameMain = new wxFrame(NULL, wxID_ANY, wxT("Wabbitemu"));// wxDefaultPosition, wxDefaultSize);
	frameMain->SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxMenuBar *m_menubar = new wxMenuBar( 0 );
	wxMenu *m_fileMenu = new wxMenu();	
	wxMenuItem* m_newMenuItem;
	m_newMenuItem = new wxMenuItem( m_fileMenu, ID_File_New, wxString( wxT("New") ) + wxT('\t') + wxT("CTRL+N"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_newMenuItem );
	
	wxMenuItem* m_openMenuItem;
	m_openMenuItem = new wxMenuItem( m_fileMenu, ID_File_Open, wxString( wxT("Open...") ) + wxT('\t') + wxT("F10"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_openMenuItem );
	
	wxMenuItem* m_saveMenuItem;
	m_saveMenuItem = new wxMenuItem( m_fileMenu, ID_File_Save, wxString( wxT("Save State...") ) , wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_saveMenuItem );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_fileMenu->AppendSeparator();
	
	wxMenuItem* m_gifMenuItem;
	m_gifMenuItem = new wxMenuItem( m_fileMenu, ID_File_Gif, wxString( wxT("Record GIF") ) + wxT('\t') + wxT("Backspace"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_gifMenuItem );
	
	wxMenuItem* m_separator2;
	m_separator2 = m_fileMenu->AppendSeparator();
	
	wxMenuItem* m_closeMenuItem;
	m_closeMenuItem = new wxMenuItem( m_fileMenu, ID_File_Close, wxString( wxT("Close") ) + wxT('\t') + wxT("CTRL+W"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_closeMenuItem );
	
	wxMenuItem* m_exitMenuItem;
	m_exitMenuItem = new wxMenuItem( m_fileMenu, ID_File_Quit, wxString( wxT("Exit") ) + wxT('\t') + wxT("ALT+F4"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_exitMenuItem );
	
	m_menubar->Append( m_fileMenu, wxT("File") );
	
	wxMenu *m_editMenu = new wxMenu();
	wxMenuItem* m_copyMenuItem;
	m_copyMenuItem = new wxMenuItem( m_editMenu, ID_Edit_Copy, wxString( wxT("Copy") ) + wxT('\t') + wxT("CTRL+C"), wxEmptyString, wxITEM_NORMAL );
	m_editMenu->Append( m_copyMenuItem );
	
	wxMenuItem* m_pasteMenuItem;
	m_pasteMenuItem = new wxMenuItem( m_editMenu, ID_Edit_Paste, wxString( wxT("Paste") ) + wxT('\t') + wxT("CTRL+V"), wxEmptyString, wxITEM_NORMAL );
	m_editMenu->Append( m_pasteMenuItem );
	
	m_menubar->Append( m_editMenu, wxT("Edit") );
	
	wxMenu *m_calcMenu = new wxMenu();
	wxMenuItem* m_skinMenuItem;
	m_skinMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Skin, wxString( wxT("Enable Skin") ) , wxEmptyString, wxITEM_CHECK );
	m_calcMenu->Append( m_skinMenuItem );
	
	wxMenuItem* m_soundMenuItem;
	m_soundMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Sound, wxString( wxT("Enable Sound") ) , wxEmptyString, wxITEM_CHECK );
	m_calcMenu->Append( m_soundMenuItem );
	
	wxMenuItem* m_separator3;
	m_separator3 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_pauseMenuItem;
	m_pauseMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Pause, wxString( wxT("Pause Emulation") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_pauseMenuItem );
	
	wxMenu *m_speedMenu = new wxMenu();
	m_calcMenu->Append( -1, wxT("Speed"), m_speedMenu );
	
	wxMenuItem* m_separator4;
	m_separator4 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_connectMenuItem;
	m_connectMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Connect, wxString( wxT("Connect To...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_connectMenuItem );
	
	wxMenuItem* m_separator6;
	m_separator6 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_varsMenuItem;
	m_varsMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Vars, wxString( wxT("Variables...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_varsMenuItem );
	
	wxMenuItem* m_optionsMenuItem;
	m_optionsMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Options, wxString( wxT("Options...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_optionsMenuItem );
	
	m_menubar->Append( m_calcMenu, wxT("Calculator") );
	
	wxMenu *m_debugMenu = new wxMenu();
	wxMenuItem* m_resetMenuItem;
	m_resetMenuItem = new wxMenuItem( m_debugMenu, ID_Debug_Reset, wxString( wxT("Reset") ) , wxEmptyString, wxITEM_NORMAL );
	m_debugMenu->Append( m_resetMenuItem );
	
	wxMenuItem* m_debugMenuItem;
	m_debugMenuItem = new wxMenuItem( m_debugMenu, ID_Debug_Open, wxString( wxT("Open Debugger...") ) , wxEmptyString, wxITEM_NORMAL );
	m_debugMenu->Append( m_debugMenuItem );
	
	m_menubar->Append( m_debugMenu, wxT("Debug") );
	
	wxMenu *m_helpMenu = new wxMenu();
	wxMenuItem* m_websiteMenuItem;
	m_websiteMenuItem = new wxMenuItem( m_helpMenu, ID_Help_Website, wxString( wxT("Website") ) , wxEmptyString, wxITEM_NORMAL );
	m_helpMenu->Append( m_websiteMenuItem );
	
	wxMenuItem* m_aboutMenuItem;
	m_aboutMenuItem = new wxMenuItem( m_helpMenu, ID_Help_About, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_helpMenu->Append( m_aboutMenuItem );
	
	m_menubar->Append( m_helpMenu, wxT("Help") );
	
	frameMain->SetMenuBar( m_menubar );
	
	wxStatusBar *m_statusBar1 = frameMain->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	frameMain->Connect(ID_File_Quit, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnFileQuit);
	frameMain->Connect(ID_Help_About, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnHelpAbout);
	frameMain->Connect(ID_Calc_Skin, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnCalcSkin);
	
	int menuSize = wxSystemSettings::GetMetric(wxSYS_MENU_Y, frameMain);
	if (calcs[slot].SkinEnabled)
		windowSize = calcs[slot].SkinSize;
	else
		windowSize.Set(128*calcs[slot].Scale, 64*calcs[slot].Scale + menuSize);
	frameMain->SetClientSize(windowSize);
}

void MyFrame::OnFileQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void MyFrame::OnCalcSkin(wxCommandEvent& event)
{
    calcs[gslot].SkinEnabled = !calcs[gslot].SkinEnabled;
	gui_frame_update(gslot);
}
 
void MyFrame::OnHelpAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("Finally works on linux"), wxT("About Wabbitemu"), wxOK | wxICON_INFORMATION, this);
}
