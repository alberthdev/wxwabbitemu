#include "gui_wx.h"
#include "calc.h"
#include "guiopenfile.h"
#include "wabbiticon.xpm"

#include "gif.h"
#include "gifhandle.h"

#define BIG_WINDOWS_ICON 0
#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b))

bool gif_anim_advance;
bool silent_mode = false;
#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
#warning wxWabbitemu benchmarking is enabled - this option will slow down the emulator.
bool BENCHMARKING = false;
bool ENABLE_GUI_UPDATING = true;
bool ENABLE_CALC_EMULATION = true;
int BENCHMARKING_HIGHEST_EMU_TIME, BENCHMARKING_LOWEST_EMU_TIME = 0;
int BENCHMARKING_HIGHEST_GUI_TIME, BENCHMARKING_LOWEST_GUI_TIME = 0;
#endif
bool DEBUG = true;
int prevCalcScale;
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
	ID_Calc_TurnCalcOn,
	ID_Calc_Vars,
	ID_Calc_Options,
	
	ID_Speed_400,
	ID_Speed_500,
	ID_Speed_200,
	ID_Speed_100,
	ID_Speed_50,
	ID_Speed_25,
	ID_Speed_Custom,
	
	ID_Size_100,
	ID_Size_200,
	ID_Size_300,
	ID_Size_400,
	
	ID_Debug_Reset,
	ID_Debug_Open,
	#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
	ID_Debug_Benchmarking,
	ID_Debug_Benchmarking_ToggleGUIUpdate,
	ID_Debug_Benchmarking_ToggleCalcEmu,
	#endif
	
	ID_Help_About,
	ID_Help_Website
};

/*BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_SHOW(MyFrame::OnShow)
	EVT_MENU(ID_Quit, MyFrame::OnQuit)
	EVT_MENU(ID_About, MyFrame::OnAbout)
END_EVENT_TABLE()*/

static MyFrame* frames[MAX_CALCS];
static wxTimer *timer;
class MyApp: public wxApp
{
	virtual bool OnInit();
	//virtual int MainLoop();
	void OnTimer(wxTimerEvent& event);
	void getTimer(int slot);
public:
};

IMPLEMENT_APP(MyApp)
char* wxStringToChar(wxString input)
{
		size_t size = input.size() + 1;
		char *buffer = new char[size];//No need to multiply by 4, converting to 1 byte char only.
#if (wxUSE_UNICODE)
		memset(buffer, 0, size); //Good Practice, Can use buffer[0] = '&#65533;' also.
		wxEncodingConverter wxec;
		wxec.Init(wxFONTENCODING_ISO8859_1, wxFONTENCODING_ISO8859_1, wxCONVERT_SUBSTITUTE);
		wxec.Convert(input.mb_str(), buffer);
		return buffer; //To free this buffer memory is user responsibility.
#else
		strcpy(buffer, input.c_str());
		return buffer;
#endif
}

//TODO: BuckeyeDude: replace with fileutilities.c
char * LoadRomIntialDialog() {
	wxString strFilter 	= wxT("\
Known types ( *.sav; *.rom)|*.sav;*.rom|\
Save States  (*.sav)|*.sav|\
ROMs  (*.rom)|*.rom|\
All Files (*.*)|*.*");
	wxFileDialog dialog(NULL, wxT("Wabbitemu: Please select a ROM or save state"),
	wxT(""), wxT(""), strFilter, wxFD_OPEN | wxFD_FILE_MUST_EXIST);//, wxDefaultPosition,wxDefaultSize, "filedlg")
	if (dialog.ShowModal() == wxID_OK) {
		return wxStringToChar(dialog.GetPath());
	}
	else
		return NULL;
}

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	printf("TPF: %d\n", TPF);
	memset(frames, 0, sizeof(frames));
	LPCALC lpCalc = calc_slot_new();
	lpCalc->SkinEnabled = false;
	MyFrame *frame;
	int result = rom_load(lpCalc, lpCalc->rom_path);
	if (result == TRUE) {
		frame = gui_frame(lpCalc);
	} else {
		calc_slot_free(lpCalc);
		char *filePath = LoadRomIntialDialog();
		//wxMessageBox(string, wxT("OnInit"), wxOK, NULL);
		//printf_d("hello", string);
		if (filePath != NULL) {
			lpCalc = calc_slot_new();
			result = rom_load(lpCalc, filePath);
			delete filePath;
			if (result == TRUE) {
				frame = gui_frame(lpCalc);
			} else {
				return FALSE;
			}
		} else {
			return FALSE;
		}
	}
	frame->SetFocus();
	timer = new wxTimer();
	timer->Connect(wxEVT_TIMER, (wxObjectEventFunction) &MyApp::OnTimer);
	timer->Start(FPS, false);
	return TRUE;
}

/*int MyApp::MainLoop() {
	
}*/

unsigned GetTickCount()
{
		struct timeval tv;
		if(gettimeofday(&tv, NULL) != 0)
			return 0;

		return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int getNumOfActiveCalcs() {
	int active_calcs = 0;
	for (int i = 0; i < MAX_CALCS; i++) {
		if (calcs[i].active) {
			active_calcs++;
		}
	}
	return active_calcs;
}

void printf_d( const char* format, ... ) {
#ifdef WXWABBITEMU_DEBUG_OUTPUT
	va_list args;
	//fprintf( stdout, "Error: " );
	va_start( args, format );
	vfprintf( stdout, format, args );
	va_end( args );
#endif
}

struct timeval initMSTimer() {
	struct timeval start;
	gettimeofday(&start, NULL);
	return start;
}

int getMSFromTimer(struct timeval startTime) {
	struct timeval end;
	long mtime,seconds,useconds;
	gettimeofday(&end, NULL);
	
	seconds = end.tv_sec - startTime.tv_sec;
	useconds = end.tv_usec - startTime.tv_usec;
	//printf_d("[getMSFromTimer] DEBUG: seconds = %ld useconds = %ld end.tv_usec = %ld \n", seconds, useconds, end.tv_usec);
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	
	return mtime;
}

void MyApp::OnTimer(wxTimerEvent& event) {
	static int difference;
	static unsigned prevTimer;
	unsigned dwTimer = GetTickCount();
	#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
	struct timeval start;
	#endif
	
	// How different the timer is from where it should be
	// guard from erroneous timer calls with an upper bound
	// that's the limit of time it will take before the
	// calc gives up and claims it lost time
	difference += ((dwTimer - prevTimer) & 0x003F) - TPF;
	prevTimer = dwTimer;
	//printf_d("%d", difference);

	// Are we greater than Ticks Per Frame that would call for
	// a frame skip?
	if (difference > -TPF) {
		#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
		// NOTE: This timer may screw up TPF counting!
		if (BENCHMARKING) gettimeofday(&start, NULL);
		if (ENABLE_CALC_EMULATION)
		#endif
		calc_run_all();
		
		#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
		if (BENCHMARKING) printf_d("[wxWabbitemu] [B] [MyApp::OnTimer] Runtime for calc_run_all() is %lu ms \n", getMSFromTimer(start));
		if (BENCHMARKING) {
			if ((BENCHMARKING_HIGHEST_EMU_TIME < (int)getMSFromTimer(start)) || ((BENCHMARKING_HIGHEST_EMU_TIME == 0 && ENABLE_CALC_EMULATION == 1)))
				BENCHMARKING_HIGHEST_EMU_TIME = (int)getMSFromTimer(start);
			if ((BENCHMARKING_LOWEST_EMU_TIME > (int)getMSFromTimer(start)) || ((BENCHMARKING_LOWEST_EMU_TIME == 0) && ENABLE_CALC_EMULATION == 1))
				BENCHMARKING_LOWEST_EMU_TIME = (int)getMSFromTimer(start);
		}
		#endif
		while (difference >= TPF) {
			calc_run_all();
			difference -= TPF;
		}

		int i;
		#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
		if (BENCHMARKING) gettimeofday(&start, NULL);
		if (ENABLE_GUI_UPDATING)
		#endif
		for (i = 0; i < MAX_CALCS; i++) {
			if (calcs[i].active) {
				frames[i]->gui_draw();
			}
		}
		#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
		if (BENCHMARKING) printf_d("[wxWabbitemu] [B] [MyApp::OnTimer] Runtime for frames[i]->gui_draw() is %lu ms \n", getMSFromTimer(start));
		if (BENCHMARKING) {
			if ((BENCHMARKING_HIGHEST_GUI_TIME < (int)getMSFromTimer(start)) || ((BENCHMARKING_HIGHEST_GUI_TIME == 0 && ENABLE_GUI_UPDATING == 1)))
				BENCHMARKING_HIGHEST_GUI_TIME = (int)getMSFromTimer(start);
			if ((BENCHMARKING_LOWEST_GUI_TIME > (int)getMSFromTimer(start)) || ((BENCHMARKING_LOWEST_GUI_TIME == 0) && ENABLE_GUI_UPDATING == 1))
				BENCHMARKING_LOWEST_GUI_TIME = (int)getMSFromTimer(start);
		}
		#endif
	// Frame skip if we're too far ahead.
	} else {
		difference += TPF;
	}
}

void MyFrame::gui_draw() {
	#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
	struct timeval start;
	if (BENCHMARKING) gettimeofday(&start, NULL);
	#endif
	wxLCD->Refresh();
	#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
	if (BENCHMARKING) printf_d("[wxWabbitemu] [B] [gui_draw] Runtime for wxLCD->Refresh(); is %lu ms \n", getMSFromTimer(start));
	if (BENCHMARKING) gettimeofday(&start, NULL);
	#endif
	wxLCD->Update();
	#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
	if (BENCHMARKING) printf_d("[wxWabbitemu] [B] [gui_draw] Runtime for wxLCD->Update(); is %lu ms \n", getMSFromTimer(start));
	if (BENCHMARKING) gettimeofday(&start, NULL);
	#endif
	//this->Refresh();
	//this->Update();

	if (lpCalc->gif_disp_state != GDS_IDLE) {
		static int skip = 0;
		if (skip == 0) {
			gif_anim_advance = true;
			this->Update();
		}
		skip = (skip + 1) % 4;
	}
	#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
	if (BENCHMARKING) printf_d("[wxWabbitemu] [B] [gui_draw] Runtime for GIF handling is %lu ms \n", getMSFromTimer(start));
	#endif
}

MyFrame *gui_frame(LPCALC lpCalc) {
	if (!lpCalc->scale)
		lpCalc->scale = 2; //Set original scale
	
	MyFrame *mainFrame = new MyFrame(lpCalc);
	mainFrame->Show(true);
	frames[lpCalc->slot] = mainFrame;

	mainFrame->wxLCD = new MyLCD(mainFrame, lpCalc);
	mainFrame->wxLCD->Show(true);
	lpCalc->running = TRUE;
	mainFrame->SetSpeed(100);
	
	mainFrame->Centre(0);   //Centres the frame
	
	mainFrame->gui_frame_update();
	return mainFrame;
}

void MyFrame::gui_frame_update() {
	wxMenuBar *wxMenu = this->GetMenuBar();
	lpCalc->calcSkin = wxGetBitmapFromMemory(TI_83p);
	lpCalc->keymap = wxGetBitmapFromMemory(TI_83p_Keymap).ConvertToImage();
	int skinWidth, skinHeight, keymapWidth, keymapHeight;
	
	if (lpCalc->calcSkin.IsOk()) {
		skinWidth = 350;//lpCalc->calcSkin.GetWidth();
		skinHeight = 725;//lpCalc->calcSkin.GetHeight();
	}
	if (lpCalc->keymap.IsOk()) {
		keymapWidth = 350;//lpCalc->keymap.GetWidth();
		keymapHeight = 725;//lpCalc->keymap.GetHeight();
	}
	int foundX = 0, foundY = 0;
	bool foundScreen = false;
	if (((skinWidth != keymapWidth) || (skinHeight != keymapHeight)) && skinHeight > 0 && skinWidth > 0) {
		lpCalc->SkinEnabled = false;
		wxMessageBox(wxT("Skin and Keymap are not the same size"), wxT("Error"),  wxOK, NULL);
	} else {
		lpCalc->SkinSize.SetWidth(skinWidth);
		lpCalc->SkinSize.SetHeight(skinHeight);		//find the screen size
		for(int y = 0; y < skinHeight && foundScreen == false; y++) {
			for (int x = 0; x < skinWidth && foundScreen == false; x++) {
				if (lpCalc->keymap.GetBlue(x, y) == 0 &&
						lpCalc->keymap.GetRed(x, y) == 255 &&
						lpCalc->keymap.GetGreen(x, y) == 0) {
					//81 92
					foundX = x;
					foundY = y;
					foundScreen = true;
				}
			}
		}
		lpCalc->LCDRect.SetLeft(foundX);
		lpCalc->LCDRect.SetTop(foundY);
		do {
			foundX++;
		} while (lpCalc->keymap.GetBlue(foundX, foundY) == 0 &&
				lpCalc->keymap.GetRed(foundX, foundY) == 255 &&
				lpCalc->keymap.GetGreen(foundX, foundY) == 0);
		lpCalc->LCDRect.SetRight(foundX--);
		do {
			foundY++;
		}while (lpCalc->keymap.GetBlue(foundX, foundY) == 0 &&
				lpCalc->keymap.GetRed(foundX, foundY) == 255 &&
				lpCalc->keymap.GetGreen(foundX, foundY) == 0);
		lpCalc->LCDRect.SetBottom(foundY);
	}
	if (!foundScreen) {
		wxMessageBox(wxT("Unable to find the screen box"), wxT("Error"), wxOK, NULL);
		lpCalc->SkinEnabled = false;
	}

	if (wxMenu != NULL) {
		if (!lpCalc->SkinEnabled) {
			wxMenu->Check(ID_Calc_Skin, false);
			// Create status bar
			wxStatusBar *wxStatus = this->GetStatusBar();
			if (wxStatus == NULL)
				wxStatus = this->CreateStatusBar(1, wxST_SIZEGRIP, wxID_ANY );
			const int iStatusWidths[] = {100, -1};
			wxStatus->SetFieldsCount(2, iStatusWidths);
			wxString wxCalcModelTxt(CalcModelTxt[lpCalc->model], wxConvUTF8);
			//wxStatus->SetStatusText(CalcModelTxt[lpCalc->model], 1);
			wxStatus->SetStatusText(wxCalcModelTxt, 1);
			
			wxSize skinSize(128*lpCalc->scale, 64*lpCalc->scale+4); //The +4 is important to show all LCD
			
			if (wxMenu)
				skinSize.IncBy(0, wxSystemSettings::GetMetric(wxSYS_MENU_Y, this));
			
			
			this->SetClientSize(skinSize);
			this->SetSize(128 * lpCalc->scale, 64 * lpCalc->scale + 60);
		} else {
			wxMenu->Check(ID_Calc_Skin, true);
			wxStatusBar *wxStatus = this->GetStatusBar();
			if (wxStatus != NULL) {
				wxStatus->Destroy();
			}
			wxSize skinSize(350, 725);
			if (wxMenu)
				skinSize.IncBy(0, wxSystemSettings::GetMetric(wxSYS_MENU_Y, this));
			this->SetClientSize(skinSize);
		}
	}
	
	this->SendSizeEvent();
}

extern wxRect db_rect;
int gui_debug(LPCALC lpCalc) {
	/*MyDebugger hdebug;
	wxRect pos = {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT+600, CW_USEDEFAULT+400};
	if (db_rect.left != -1) CopyRect(&pos, &db_rect);

	pos.right -= pos.left;
	pos.bottom -= pos.top;

	if ((hdebug = FindWindow(g_szDebugName, "Debugger"))) {
		SwitchToThisWindow(hdebug, TRUE);
		return -1;
	}
	lpCalc->running = FALSE;
	hdebug = CreateWindowEx(
		WS_EX_APPWINDOW,
		g_szDebugName,
		"Debugger",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		pos.left, pos.top, pos.right, pos.bottom,
		0, 0, g_hInst, NULL);

	lpCalc->hwndDebug = hdebug;
	SendMessage(hdebug, WM_SIZE, 0, 0);*/
	return 0;
}

MyFrame::MyFrame(LPCALC lpCalc) : wxFrame(NULL, wxID_ANY, wxT("Wabbitemu"))
{
	this->lpCalc = lpCalc;
	this->SetWindowStyleFlag(wxBORDER_RAISED);
	wxSize skinSize(350, 725);
	lpCalc->SkinSize = skinSize;
	LCD_t *lcd = lpCalc->cpu.pio.lcd;
	int scale = lpCalc->scale;
	int draw_width = lcd->width * scale;
	int draw_height = 64 * scale;
	wxRect lcdRect((128 * scale - draw_width) / 2, 0, draw_width, draw_height);
	lpCalc->LCDRect = lcdRect;

	wxSize windowSize;
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxMenuBar *m_menubar = new wxMenuBar( 0 );
	wxMenu *m_fileMenu = new wxMenu();	
	wxMenuItem* m_newMenuItem;
	m_newMenuItem = new wxMenuItem( m_fileMenu, ID_File_New, wxString( wxT("New") ) + wxT('\t') + wxT("CTRL+N"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_newMenuItem );
	
	wxMenuItem* m_openMenuItem;
	m_openMenuItem = new wxMenuItem( m_fileMenu, ID_File_Open, wxString( wxT("Open...") ) + wxT('\t') + wxT("CTRL+O"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_openMenuItem );
	
	wxMenuItem* m_saveMenuItem;
	m_saveMenuItem = new wxMenuItem( m_fileMenu, ID_File_Save, wxString( wxT("Save State...") ) , wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_saveMenuItem );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_fileMenu->AppendSeparator();
	
	wxMenuItem* m_gifMenuItem;
	m_gifMenuItem = new wxMenuItem( m_fileMenu, ID_File_Gif, wxString( wxT("Record GIF") ) + wxT('\t') + wxT("Backspace"), wxEmptyString, wxITEM_CHECK );
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
	m_editMenu->Append( m_pasteMenuItem );this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxMenuItem* m_separator5;
	m_separator5 = m_editMenu->AppendSeparator();
	
	wxMenuItem* m_optionsMenuItem;
	m_optionsMenuItem = new wxMenuItem( m_editMenu, ID_Calc_Options, wxString( wxT("Preferences\tCtrl+Q") ) , wxEmptyString, wxITEM_NORMAL );
	m_editMenu->Append( m_optionsMenuItem );
	
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
	m_pauseMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Pause, wxString( wxT("Pause Emulation") ) , wxEmptyString, wxITEM_CHECK );
	m_calcMenu->Append( m_pauseMenuItem );
	
	wxMenu *m_speedMenu = new wxMenu();
	m_calcMenu->Append( -1, wxT("Speed"), m_speedMenu );
	
	wxMenuItem* m_setSpeed500;
	m_setSpeed500 = new wxMenuItem( m_speedMenu, ID_Speed_500, wxString( wxT("500%") ) , wxEmptyString, wxITEM_CHECK );
	m_speedMenu->Append( m_setSpeed500 );
	
	wxMenuItem* m_setSpeed400;
	m_setSpeed400 = new wxMenuItem( m_speedMenu, ID_Speed_400, wxString( wxT("400%") ) , wxEmptyString, wxITEM_CHECK );
	m_speedMenu->Append( m_setSpeed400 );
	
	wxMenuItem* m_setSpeed200;
	m_setSpeed200 = new wxMenuItem( m_speedMenu, ID_Speed_200, wxString( wxT("200%") ) , wxEmptyString, wxITEM_CHECK );
	m_speedMenu->Append( m_setSpeed200 );
	
	wxMenuItem* m_setSpeed100;
	m_setSpeed100 = new wxMenuItem( m_speedMenu, ID_Speed_100, wxString( wxT("100%") ) , wxEmptyString, wxITEM_CHECK );
	m_speedMenu->Append( m_setSpeed100 );
	
	wxMenuItem* m_setSpeed50;
	m_setSpeed50 = new wxMenuItem( m_speedMenu, ID_Speed_50, wxString( wxT("50%") ) , wxEmptyString, wxITEM_CHECK );
	m_speedMenu->Append( m_setSpeed50 );
	
	wxMenuItem* m_setSpeed25;
	m_setSpeed25 = new wxMenuItem( m_speedMenu, ID_Speed_25, wxString( wxT("25%") ) , wxEmptyString, wxITEM_CHECK );
	m_speedMenu->Append( m_setSpeed25 );
	
	wxMenuItem* m_separatorSpeedCustom;
	m_separatorSpeedCustom = m_speedMenu->AppendSeparator();
	
	wxMenuItem* m_setSpeedCustom;
	m_setSpeedCustom = new wxMenuItem( m_speedMenu, ID_Speed_Custom, wxString( wxT("Custom...") ) , wxEmptyString, wxITEM_CHECK );
	m_speedMenu->Append( m_setSpeedCustom );
	
	wxMenu *m_sizeMenu = new wxMenu();
	m_calcMenu->Append(-1,wxT("Size"), m_sizeMenu);
	
	wxMenuItem* m_setSize100;
	m_setSize100 = new wxMenuItem( m_sizeMenu, ID_Size_100, wxString( wxT("100%") ) , wxEmptyString, wxITEM_CHECK );
	m_sizeMenu->Append( m_setSize100 );
	
	wxMenuItem* m_setSize200;
	m_setSize200 = new wxMenuItem( m_sizeMenu, ID_Size_200, wxString( wxT("200%") ) , wxEmptyString, wxITEM_CHECK );
	m_sizeMenu->Append( m_setSize200 );
	
	wxMenuItem* m_setSize300;
	m_setSize300 = new wxMenuItem( m_sizeMenu, ID_Size_300, wxString( wxT("300%") ) , wxEmptyString, wxITEM_CHECK );
	m_sizeMenu->Append( m_setSize300 );
	
	wxMenuItem* m_setSize400;
	m_setSize400 = new wxMenuItem( m_sizeMenu, ID_Size_400, wxString( wxT("400%") ) , wxEmptyString, wxITEM_CHECK );
	m_sizeMenu->Append( m_setSize400 );
	
	wxMenuItem* m_separator4;
	m_separator4 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_connectMenuItem;
	m_connectMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Connect, wxString( wxT("Connect To...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_connectMenuItem );
	
	wxMenuItem* m_separator7;
	m_separator7 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_TurnOnCalcMenuItem;
	m_TurnOnCalcMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_TurnCalcOn, wxString( wxT("Turn Calculator On...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_TurnOnCalcMenuItem );
	
	wxMenuItem* m_separator6;
	m_separator6 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_varsMenuItem;
	m_varsMenuItem = new wxMenuItem( m_calcMenu, ID_Calc_Vars, wxString( wxT("Variables...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_varsMenuItem );
	
	m_menubar->Append( m_calcMenu, wxT("Calculator") );
	
	wxMenu *m_debugMenu = new wxMenu();
	wxMenuItem* m_resetMenuItem;
	m_resetMenuItem = new wxMenuItem( m_debugMenu, ID_Debug_Reset, wxString( wxT("Reset") ) , wxEmptyString, wxITEM_NORMAL );
	m_debugMenu->Append( m_resetMenuItem );
	
	wxMenuItem* m_debugMenuItem;
	m_debugMenuItem = new wxMenuItem( m_debugMenu, ID_Debug_Open, wxString( wxT("Open Debugger...") ) , wxEmptyString, wxITEM_NORMAL );
	m_debugMenu->Append( m_debugMenuItem );
	
	#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
	wxMenuItem* m_benchmarkMenuItem;
	m_benchmarkMenuItem = new wxMenuItem( m_debugMenu, ID_Debug_Benchmarking, wxString( wxT("Emulation Benchmarking (slow)") ) , wxEmptyString, wxITEM_CHECK );
	m_debugMenu->Append( m_benchmarkMenuItem );
	
	wxMenu *m_BenchmarkingOptsMenu = new wxMenu();
	m_debugMenu->Append(-1, wxT("Benchmarking Options"), m_BenchmarkingOptsMenu);
	
	wxMenuItem* m_toggleGUIUpdateMenuItem;
	m_toggleGUIUpdateMenuItem = new wxMenuItem( m_BenchmarkingOptsMenu, ID_Debug_Benchmarking_ToggleGUIUpdate, wxString( wxT("GUI Updating") ) , wxEmptyString, wxITEM_CHECK );
	m_BenchmarkingOptsMenu->Append( m_toggleGUIUpdateMenuItem );
	m_toggleGUIUpdateMenuItem->Check();
	m_toggleGUIUpdateMenuItem->Enable(false);
	
	wxMenuItem* m_toggleCalcEmuMenuItem;
	m_toggleCalcEmuMenuItem = new wxMenuItem( m_BenchmarkingOptsMenu, ID_Debug_Benchmarking_ToggleCalcEmu, wxString( wxT("Calculator Emulation") ) , wxEmptyString, wxITEM_CHECK );
	m_BenchmarkingOptsMenu->Append( m_toggleCalcEmuMenuItem );
	m_toggleCalcEmuMenuItem->Check();
	m_toggleCalcEmuMenuItem->Enable(false);
	#endif
	
	m_menubar->Append( m_debugMenu, wxT("Debug") );
	
	wxMenu *m_helpMenu = new wxMenu();
	wxMenuItem* m_websiteMenuItem;
	m_websiteMenuItem = new wxMenuItem( m_helpMenu, ID_Help_Website, wxString( wxT("Website") ) , wxEmptyString, wxITEM_NORMAL );
	m_helpMenu->Append( m_websiteMenuItem );
	
	wxMenuItem* m_aboutMenuItem;
	m_aboutMenuItem = new wxMenuItem( m_helpMenu, ID_Help_About, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_helpMenu->Append( m_aboutMenuItem );
	
	m_menubar->Append( m_helpMenu, wxT("Help") );
	
	this->SetMenuBar( m_menubar );
	
	wxStatusBar *m_statusBar1 = new wxStatusBar(this);
	this->SetStatusBar(m_statusBar1);
	
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(MyFrame::OnPaint));
	// Resize event connection
	this->Connect(wxEVT_SIZE, (wxObjectEventFunction) &MyFrame::OnResize);
	
	this->Connect(ID_File_New, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnFileNew);
	this->Connect(ID_File_Open, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnFileOpen);
	this->Connect(ID_File_Save, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnFileSave);
	this->Connect(ID_File_Close, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnFileClose);
	this->Connect(ID_File_Gif, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnGIF);
	this->Connect(ID_File_Quit, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnFileQuit);
	this->Connect(ID_Calc_Pause, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnPauseEmulation);
	this->Connect(ID_Calc_TurnCalcOn, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnTurnCalcOn);
	
	this->Connect(ID_Speed_Custom, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSpeedCustom);
	this->Connect(ID_Speed_500, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSpeed);
	this->Connect(ID_Speed_400, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSpeed);
	this->Connect(ID_Speed_200, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSpeed);
	this->Connect(ID_Speed_100, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSpeed);
	this->Connect(ID_Speed_50, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSpeed);
	this->Connect(ID_Speed_25, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSpeed);
	
	this->Connect(ID_Size_100, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSize);
	this->Connect(ID_Size_200, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSize);
	this->Connect(ID_Size_300, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSize);
	this->Connect(ID_Size_400, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnSetSize);
	
	#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
	this->Connect(ID_Debug_Benchmarking, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnBenchmarking);
	this->Connect(ID_Debug_Benchmarking_ToggleGUIUpdate, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnBenchToggleGUIUpdate);
	this->Connect(ID_Debug_Benchmarking_ToggleCalcEmu, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnBenchToggleCalcEmu);
	#endif
	
	this->Connect(ID_Help_About, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnHelpAbout);
	this->Connect(ID_Help_Website, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnHelpWebsite);
	this->Connect(ID_Calc_Skin, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MyFrame::OnCalcSkin);
	this->Connect(wxID_ANY, wxEVT_KEY_DOWN, (wxObjectEventFunction) &MyFrame::OnKeyDown);
	this->Connect(wxID_ANY, wxEVT_KEY_UP, (wxObjectEventFunction) &MyFrame::OnKeyUp);
	/* OnQuit */
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrame::OnQuit));
	
	//Speed starts at 100%
	m_menubar->Check(ID_Speed_100, true);
	//Size starts at 200%
	m_menubar->Check(ID_Size_200, true);
	
	//int menuSize = wxSystemSettings::GetMetric(wxSYS_MENU_Y);
	if (lpCalc->SkinEnabled) {
		windowSize = lpCalc->SkinSize;
	} else {
	//what is this +60?
		windowSize.Set(128 * scale, 64 * scale + 60);
	}
	
	#if (defined(__WXMSW__) && BIG_WINDOWS_ICON == 1)
	wxBitmap bitmap(wxT(“wabbiticon.png”), wxBITMAP_TYPE_PNG);
	icon.CopyFromBitmap(bitmap);
	#else
	wxIcon icon(wxICON(wabbiticon));
	#endif
	
	SetIcon(icon);
	
	this->SetSize(windowSize);
}

// Resize function
void MyFrame::OnResize(wxSizeEvent& event) {
	event.Skip(true);
	if (lpCalc->SkinEnabled)
		return;
	int width_scale = event.GetSize().GetWidth() / 128;
	int height_scale = (event.GetSize().GetHeight()-60) / 64;
	int scale = max(2, max(width_scale, height_scale));

	/*int new_width = event.GetSize().GetWidth();
	int new_height = event.GetSize().GetHeight();
	if (new_width > this->GetSize().GetWidth() || new_height > this->GetSize().GetHeight())
		scale++;
	else
		scale--;*/
	lpCalc->scale = max(2, scale);
	this->SetSize(wxDefaultCoord, wxDefaultCoord, scale * 128, scale * 64 + 60, wxSIZE_USE_EXISTING);
}

void MyFrame::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	if (lpCalc->SkinEnabled) {
		this->wxLCD->Update();
		wxBitmap test = wxGetBitmapFromMemory(TI_83p);
		dc.DrawBitmap(test, 0, 0, true);
	}
}

void MyFrame::OnFileNew(wxCommandEvent &event) {
	printf_d("[wxWabbitemu] [OnFileNew] Function called!\n");
	printf_d("[wxWabbitemu] [OnFileNew] Creating another calc...\n");
	LPCALC lpCalcNew = calc_slot_new();
	printf_d("[wxWabbitemu] [OnFileNew] Loading the same ROM into new calc...\n");
	if (rom_load(lpCalcNew, lpCalc->rom_path)) {
		printf_d("[wxWabbitemu] [OnFileNew] Setting new calc attributes...\n");
		lpCalcNew->SkinEnabled = lpCalc->SkinEnabled;
		lpCalcNew->bCutout = lpCalc->bCutout;
		lpCalcNew->scale = lpCalc->scale;
		/* Nope, this doesn't exist yet... :/ */
		//lpCalcNew->FaceplateColor = lpCalc->FaceplateColor;
		//lpCalcNew->bAlphaBlendLCD = lpCalc->bAlphaBlendLCD;
		printf_d("[wxWabbitemu] [OnFileNew] Turning on new calc... (disabled due to hanging)\n");
		//calc_turn_on(lpCalcNew);
		printf_d("[wxWabbitemu] [OnFileNew] Creating new window for the new calc...\n");
		gui_frame(lpCalcNew);
	} else {
		calc_slot_free(lpCalcNew);
		wxMessageBox(wxT("Failed to create new calc!"));
	}
}

void MyFrame::OnFileOpen(wxCommandEvent &event) {
	GetOpenSendFileName(lpCalc, 0);
}

void MyFrame::OnFileSave(wxCommandEvent &event) {
	SaveStateDialog(lpCalc);
}

void MyFrame::OnFileClose(wxCommandEvent &event) {
	Close(TRUE);
}

void MyFrame::OnSetSize(wxCommandEvent &event) {
	/* This function is called when user changes size of LCD in menu */
	wxMenuBar *wxMenu = this->GetMenuBar();
	if (lpCalc->SkinEnabled) {
		wxMessageBox(wxString(wxT("Hey there sneaky one! You got past the menu! Unfortunately, we can't let you past - otherwise things would break! Sorry, but nice try.")), wxString(wxT("Wabbitemu - Gotcha!")), wxOK, this);
	} else {
		int eventID;
		wxMenu->Check(ID_Size_100,false);
		wxMenu->Check(ID_Size_200,false);
		wxMenu->Check(ID_Size_300,false);
		wxMenu->Check(ID_Size_400,false);
		
		eventID = event.GetId();
		
		switch (eventID) {
			case ID_Size_100:
				lpCalc->scale = 1;  //This is half of the Wabbit default, but equals real LCD
				wxMenu->Check(ID_Size_100,true);
				printf_d("[wxWabbitemu] [OnSetSize] Set Scale 100% \n");
				break;
			case ID_Size_200:
				lpCalc->scale = 2; //Wabbit default, twice the LCD
				wxMenu->Check(ID_Size_200,true);
				printf_d("[wxWabbitemu] [OnSetSize] Set Scale 200% \n");
				break;
			case ID_Size_300:
				lpCalc->scale = 3;
				wxMenu->Check(ID_Size_300,true);
				printf_d("[wxWabbitemu] [OnSetSize] Set Scale 300% \n");
				break;
			case ID_Size_400:
				lpCalc->scale = 4;
				wxMenu->Check(ID_Size_400,true);
				printf_d("[wxWabbitemu] [OnSetSize] Set Scale 400% \n");
				break;
			default:
			printf_d("[wxWabbitemu] [W] [OnSetSize] Some strange, evil thing called this function. Disregarding. \n");
			break;
		}
		if (!lpCalc->SkinEnabled) {
			/* Update size of frame to match the new LCD Size */
			//why +60
			this->SetSize(128 * lpCalc->scale, 64 * lpCalc->scale + 60);
		}
	}
}

void MyFrame::OnSetSpeed(wxCommandEvent &event) {
	printf_d("[wxWabbitemu] [OnSetSpeed] Function called! \n");
	wxMenuBar *wxMenu = this->GetMenuBar();
	int eventID;
	wxMenu->Check(ID_Speed_500, false);
	wxMenu->Check(ID_Speed_400, false);
	wxMenu->Check(ID_Speed_200, false);
	wxMenu->Check(ID_Speed_100, false);
	wxMenu->Check(ID_Speed_50, false);
	wxMenu->Check(ID_Speed_25, false);
	wxMenu->Check(ID_Speed_Custom, false);
	wxMenu->SetLabel(ID_Speed_Custom, wxString(wxT("Custom...")));
	wxMenu->Check(ID_Speed_Custom, false);
	
	eventID = event.GetId();
	printf_d("[wxWabbitemu] [OnSetSpeed] Got widget ID that called this function: %d \n",eventID);
	switch (eventID) {
		case ID_Speed_100:
			this->SetSpeed(100);
			wxMenu->Check(ID_Speed_100, true);
			printf_d("[wxWabbitemu] [OnSetSpeed] Setting emulated calc speed to 100%%. \n");
			break;
		case ID_Speed_200:
			this->SetSpeed(200);
			wxMenu->Check(ID_Speed_200, true);
			printf_d("[wxWabbitemu] [OnSetSpeed] Setting emulated calc speed to 200%%. \n");
			break;
		case ID_Speed_25:
			this->SetSpeed(25);
			wxMenu->Check(ID_Speed_25, true);
			printf_d("[wxWabbitemu] [OnSetSpeed] Setting emulated calc speed to 25%%. \n");
			break;
		case ID_Speed_400:
			this->SetSpeed(400);
			wxMenu->Check(ID_Speed_400, true);
			printf_d("[wxWabbitemu] [OnSetSpeed] Setting emulated calc speed to 400%%. \n");
			break;
		case ID_Speed_50:
			this->SetSpeed(50);
			wxMenu->Check(ID_Speed_50, true);
			printf_d("[wxWabbitemu] [OnSetSpeed] Setting emulated calc speed to 50%%. \n");
			break;
		case ID_Speed_500:
			this->SetSpeed(500);
			wxMenu->Check(ID_Speed_500, true);
			printf_d("[wxWabbitemu] [OnSetSpeed] Setting emulated calc speed to 500%%. \n");
			break;
		default:
			printf_d("[wxWabbitemu] [W] [OnSetSpeed] Some strange, evil thing called this function. Disregarding. \n");
			break;
	}
}

void MyFrame::OnSetSpeedCustom(wxCommandEvent &event) {
	wxMenuBar *wxMenu = this->GetMenuBar();
	long resp;
	printf_d("[wxWabbitemu] [OnSetSpeedCustom] Function called! Opening numerical input dialog...\n");
	resp = wxGetNumberFromUser(wxString(wxT("Enter the speed (in percentage) you wish to set:")), wxString(wxT("")), wxString(wxT("Wabbitemu - Custom Speed")), 100, 0, 10000);
	if (resp != -1) {
		printf_d("[wxWabbitemu] [OnSetSpeedCustom] User provided a valid number, so setting speed %%. \n");
		wxMenu->SetLabel(ID_Speed_Custom, wxString::Format(wxT("%i%%"),resp));
		wxMenu->Check(ID_Speed_500, false);
		wxMenu->Check(ID_Speed_400, false);
		wxMenu->Check(ID_Speed_200, false);
		wxMenu->Check(ID_Speed_100, false);
		wxMenu->Check(ID_Speed_50, false);
		wxMenu->Check(ID_Speed_25, false);
		wxMenu->Check(ID_Speed_Custom, true);
		this->SetSpeed(resp);
		printf_d("[wxWabbitemu] [OnSetSpeedCustom] Speed set to %i%%. \n", resp);
	} else {
		// note to self: does the "entered something invalid" apply?? supposedly, OnSetSpeedCustom is checked...
		printf_d("[wxWabbitemu] [OnSetSpeedCustom] User canceled dialog or entered something invalid. \n", resp);
		/* Dirty, evil hack... but I'm too lazy to create another var to indicate
		 * custom speed status, soo... :P */
		if (wxMenu->GetLabel(ID_Speed_Custom) == wxString(wxT("Custom..."))) {
			// Do nothing
			printf_d("[wxWabbitemu] [OnSetSpeedCustom] Menu label is 'Custom...', so unchecking the menu. \n", resp);
			wxMenu->Check(ID_Speed_Custom, false);
		} else {
			printf_d("[wxWabbitemu] [OnSetSpeedCustom] Menu label is different, so checking the menu. \n");
			wxMenu->Check(ID_Speed_Custom, true);
		}
	}
}

void MyFrame::OnPauseEmulation(wxCommandEvent &event) {
	wxMenuBar *wxMenu = this->GetMenuBar();
	if (lpCalc->running) {
		//Tick is checked and emulation stops
		lpCalc->running = FALSE;
		wxMenu->Check(ID_Calc_Pause, true);
	} else {
		//Tick is unchecked and emulation resumes
		lpCalc->running = TRUE;
		wxMenu->Check(ID_Calc_Pause, false);
	}
}

void MyFrame::SetSpeed(int speed) {
	lpCalc->speed = speed;
	//wxMenuBar *wxMenu = this->GetMenuBar();
}

void MyFrame::OnTurnCalcOn(wxCommandEvent& event) {
	lpCalc->cpu.pio.keypad->on_pressed |= KEY_FALSEPRESS;
	lpCalc->cpu.pio.keypad->on_pressed &= ~KEY_FALSEPRESS;
	
	if (!lpCalc->cpu.pio.lcd->active)
		wxMessageBox(wxT("Could not turn calculator on!"), wxT("wxWabbitemu Error"), wxOK | wxICON_ERROR, this);
}

#ifdef WXWABBITEMU_DEBUG_BENCHMARKING
void MyFrame::OnBenchmarking(wxCommandEvent& event) {
	printf_d("[wxWabbitemu] [B] [OnBenchmarking] Function called!\n");
	wxMenuBar *wxMenu = this->GetMenuBar();
	BENCHMARKING = !BENCHMARKING;
	if (BENCHMARKING) {
		printf_d("[wxWabbitemu] [B] [OnBenchmarking] Benchmarking enabled! Emulator may become very slow.\n");
	} else {
		printf_d("[wxWabbitemu] [B] [OnBenchmarking] Benchmarking disabled!\n");
		printf_d("[wxWabbitemu] [B] [OnBenchmarking] Benchmarking statistics:\n");
		printf_d("[wxWabbitemu] [B] [OnBenchmarking] GUI updating: longest time is %ims, shortest time %ims\n", BENCHMARKING_HIGHEST_GUI_TIME, BENCHMARKING_LOWEST_GUI_TIME);
		printf_d("[wxWabbitemu] [B] [OnBenchmarking] Calc emulation: longest time is %ims, shortest time %ims\n", BENCHMARKING_HIGHEST_EMU_TIME, BENCHMARKING_LOWEST_EMU_TIME);
	}
	wxMenu->Check(ID_Debug_Benchmarking, BENCHMARKING);
	wxMenu->Enable(ID_Debug_Benchmarking_ToggleCalcEmu, BENCHMARKING);
	wxMenu->Enable(ID_Debug_Benchmarking_ToggleGUIUpdate, BENCHMARKING);
}

void MyFrame::OnBenchToggleGUIUpdate(wxCommandEvent& event) {
	printf_d("[wxWabbitemu] [B] [OnBenchToggleGUIUpdate] Function called!\n");
	wxMenuBar *wxMenu = this->GetMenuBar();
	ENABLE_GUI_UPDATING = !ENABLE_GUI_UPDATING;
	if (ENABLE_GUI_UPDATING) {
		printf_d("[wxWabbitemu] [B] [OnBenchToggleGUIUpdate] GUI updating enabled!\n");
	} else {
		printf_d("[wxWabbitemu] [B] [OnBenchToggleGUIUpdate] GUI updating disabled! The GUI LCD will no longer be updated.\n");
	}
	wxMenu->Check(ID_Debug_Benchmarking_ToggleGUIUpdate, ENABLE_GUI_UPDATING);
}

void MyFrame::OnBenchToggleCalcEmu(wxCommandEvent& event) {
	printf_d("[wxWabbitemu] [B] [OnBenchToggleCalcEmu] Function called!\n");
	wxMenuBar *wxMenu = this->GetMenuBar();
	ENABLE_CALC_EMULATION = !ENABLE_CALC_EMULATION;
	if (ENABLE_CALC_EMULATION) {
		printf_d("[wxWabbitemu] [B] [OnBenchToggleCalcEmu] Calculator emulation enabled!\n");
	} else {
		printf_d("[wxWabbitemu] [B] [OnBenchToggleCalcEmu] Calculator emulation disabled! Some functions of the program may be impaired.\n");
	}
	wxMenu->Check(ID_Debug_Benchmarking_ToggleCalcEmu, ENABLE_CALC_EMULATION);
}
#endif

void MyFrame::OnKeyDown(wxKeyEvent& event)
{
	int keycode = event.GetKeyCode();
	if (keycode == WXK_F8) {
		if (lpCalc->speed == 100)
			SetSpeed(400);
		else
			SetSpeed(100);
	}
	if (keycode == WXK_SHIFT) {
		wxUint32 raw = event.GetRawKeyCode();
		if (raw == 65505) {
			keycode = WXK_LSHIFT;
		} else {
			keycode = WXK_RSHIFT;
		}
	}

	keyprog_t *kp = keypad_key_press(&lpCalc->cpu, keycode);
	if (kp) {
		if ((lpCalc->cpu.pio.keypad->keys[kp->group][kp->bit] & KEY_STATEDOWN) == 0) {
			lpCalc->cpu.pio.keypad->keys[kp->group][kp->bit] |= KEY_STATEDOWN;
			this->Update();
			FinalizeButtons();
		}
	}

}

void MyFrame::OnKeyUp(wxKeyEvent& event)
{
	int key = event.GetKeyCode();
	if (key == WXK_SHIFT) {
		keypad_key_release(&lpCalc->cpu, WXK_LSHIFT);
		keypad_key_release(&lpCalc->cpu, WXK_RSHIFT);
	} else {
		keypad_key_release(&lpCalc->cpu, key);
	}
	FinalizeButtons();
}

void MyFrame::OnFileQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void MyFrame::OnCalcSkin(wxCommandEvent& event)
{
	wxMenuBar *wxMenu = this->GetMenuBar();
	lpCalc->SkinEnabled = !lpCalc->SkinEnabled;
	
	// Enable/disable sizing menu items.
	//m_sizeMenu->Enable(!lpCalc->SkinEnabled);
	wxMenu->Enable(ID_Size_100, !lpCalc->SkinEnabled);
	wxMenu->Enable(ID_Size_200, !lpCalc->SkinEnabled);
	wxMenu->Enable(ID_Size_300, !lpCalc->SkinEnabled);
	wxMenu->Enable(ID_Size_400, !lpCalc->SkinEnabled);
	
	// Connect or disconnect the OnResize event.
	if (lpCalc->SkinEnabled) {
		prevCalcScale = lpCalc->scale;
		lpCalc->scale = 2;
		this->Disconnect(wxEVT_SIZE, (wxObjectEventFunction) &MyFrame::OnResize);
	} else {
		lpCalc->scale = prevCalcScale;
		this->Connect(wxEVT_SIZE, (wxObjectEventFunction) &MyFrame::OnResize);
	}
	gui_frame_update();
	this->Refresh();
	this->Update();
}
 
void MyFrame::OnHelpAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxT("wxWabbitEmu is a port of Wabbitemu that is cross-platform."), wxT("About Wabbitemu"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnHelpWebsite(wxCommandEvent& WXUNUSED(event))
{
	//This function is currently linux only
	system("xdg-open http://code.google.com/p/wxwabbitemu/");
}

void MyFrame::OnGIF(wxCommandEvent& WXUNUSED(event))
{
	printf_d("[wxWabbitemu] OnGIF called!\n");
	wxMenuBar *wxMenu = this->GetMenuBar();
	// Now to actually do something!
	if (gif_write_state == GIF_IDLE) {
		wxFileDialog* saveGIFDialog = new wxFileDialog(NULL, wxT("Save GIF file"), wxT(""), wxT(""), wxT("\
GIF File (*.gif)|*.GIF;*.gif;*.Gif;*.GIf;*.gIf;*.gIF;*.giF|\
All Files (*.*)|*.*\0"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
		if (saveGIFDialog->ShowModal() != wxID_OK) {
			wxMenu->Check(ID_File_Gif, false);
			return;
		}
		wxString path;
		path.append( saveGIFDialog->GetDirectory() );
		path.append( wxFileName::GetPathSeparator() );
		path.append( saveGIFDialog->GetFilename() );
		printf("Got path: %s \n", (const char*)path.mb_str());
		strcpy(gif_file_name, (const char*)path.mb_str());
		gif_write_state = GIF_START;
		for (int i = 0; i < MAX_CALCS; i++)
			if (lpCalc->active)
				lpCalc->gif_disp_state = GDS_STARTING;
		wxMenu->Check(ID_File_Gif, true);
	} else {
		gif_write_state = GIF_END;
		for (int i = 0; i < MAX_CALCS; i++)
			if (lpCalc->active)
				lpCalc->gif_disp_state = GDS_ENDING;
		wxMenu->Check(ID_File_Gif, false);
	}
}

void MyFrame::OnQuit(wxCloseEvent& event)
{
	printf_d("[wxWabbitemu] OnQuit called! \n");
	/* Created event in preparation to fix crash bug - this should NOT
	 * affect normal operation. */
	printf_d("[wxWabbitemu] [OnQuit] Killing emulation/GUI update timer(s)... \n");
	printf_d("[wxWabbitemu] [OnQuit] Number of active calcs [getNumOfActiveCalcs() value]: %i \n", getNumOfActiveCalcs());
	if (getNumOfActiveCalcs() <= 1) {
		timer->Stop();
	} else {
		printf_d("[wxWabbitemu] [OnQuit] Not killing emulation/GUI update timer since other calcs are still running. \n");
	}
	lpCalc->active = FALSE;
	//wxTimer *thetimer = this.timer;
	//wxTimer *thetimer = MyApp::timer;
	calc_slot_free(lpCalc);
	Destroy();
}

void MyFrame::FinalizeButtons() {
	int group, bit;
	keypad_t *kp = lpCalc->cpu.pio.keypad;
	for(group = 0; group < 7; group++) {
		for(bit = 0; bit < 8; bit++) {
			if ((kp->keys[group][bit] & KEY_STATEDOWN) &&
				((kp->keys[group][bit] & KEY_MOUSEPRESS) == 0) &&
				((kp->keys[group][bit] & KEY_KEYBOARDPRESS) == 0)) {
					kp->keys[group][bit] &= (~KEY_STATEDOWN);
			}
		}
	}
}

int SetGIFName() {
	wxFileDialog* saveGIFDialog = new wxFileDialog(NULL, wxT("Save GIF file"), wxT(""), wxT(""), wxT("\
GIF File (*.gif)|*.GIF;*.gif;*.Gif;*.GIf;*.gIf;*.gIF;*.giF|\
All Files (*.*)|*.*\0"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
	if (saveGIFDialog->ShowModal() != wxID_OK)
		return false;
	wxString path;
	path.append( saveGIFDialog->GetDirectory() );
	path.append( wxFileName::GetPathSeparator() );
	path.append( saveGIFDialog->GetFilename() );
	printf("Got path: %s \n", (const char*)path.mb_str());
	strcpy(gif_file_name, (const char*)path.mb_str());
	return true;
}
