///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxgui.h"

///////////////////////////////////////////////////////////////////////////

BOOL gif_anim_advance;
BOOL silent_mode = FALSE;

wxWabbitemu::wxWabbitemu( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar = new wxMenuBar( 0 );
	m_fileMenu = new wxMenu();
	wxMenuItem* m_newMenuItem;
	m_newMenuItem = new wxMenuItem( m_fileMenu, wxID_ANY, wxString( wxT("New") ) + wxT('\t') + wxT("CTRL+N"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_newMenuItem );
	
	wxMenuItem* m_openMenuItem;
	m_openMenuItem = new wxMenuItem( m_fileMenu, wxID_ANY, wxString( wxT("Open...") ) + wxT('\t') + wxT("F10"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_openMenuItem );
	
	wxMenuItem* m_saveMenuItem;
	m_saveMenuItem = new wxMenuItem( m_fileMenu, wxID_ANY, wxString( wxT("Save State...") ) , wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_saveMenuItem );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_fileMenu->AppendSeparator();
	
	wxMenuItem* m_gifMenuItem;
	m_gifMenuItem = new wxMenuItem( m_fileMenu, wxID_ANY, wxString( wxT("Record GIF") ) + wxT('\t') + wxT("Backspace"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_gifMenuItem );
	
	wxMenuItem* m_separator2;
	m_separator2 = m_fileMenu->AppendSeparator();
	
	wxMenuItem* m_closeMenuItem;
	m_closeMenuItem = new wxMenuItem( m_fileMenu, wxID_ANY, wxString( wxT("Close") ) + wxT('\t') + wxT("CTRL+W"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_closeMenuItem );
	
	wxMenuItem* m_exitMenuItem;
	m_exitMenuItem = new wxMenuItem( m_fileMenu, wxID_ANY, wxString( wxT("Exit") ) + wxT('\t') + wxT("ALT+F4"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_exitMenuItem );
	
	m_menubar->Append( m_fileMenu, wxT("File") );
	
	m_editMenu = new wxMenu();
	wxMenuItem* m_copyMenuItem;
	m_copyMenuItem = new wxMenuItem( m_editMenu, wxID_ANY, wxString( wxT("Copy") ) + wxT('\t') + wxT("CTRL+C"), wxEmptyString, wxITEM_NORMAL );
	m_editMenu->Append( m_copyMenuItem );
	
	wxMenuItem* m_pasteMenuItem;
	m_pasteMenuItem = new wxMenuItem( m_editMenu, wxID_ANY, wxString( wxT("Paste") ) + wxT('\t') + wxT("CTRL+V"), wxEmptyString, wxITEM_NORMAL );
	m_editMenu->Append( m_pasteMenuItem );
	
	m_menubar->Append( m_editMenu, wxT("Edit") );
	
	m_calcMenu = new wxMenu();
	wxMenuItem* m_skinMenuItem;
	m_skinMenuItem = new wxMenuItem( m_calcMenu, wxID_ANY, wxString( wxT("Enable Skin") ) , wxEmptyString, wxITEM_CHECK );
	m_calcMenu->Append( m_skinMenuItem );
	
	wxMenuItem* m_soundMenuItem;
	m_soundMenuItem = new wxMenuItem( m_calcMenu, wxID_ANY, wxString( wxT("Enable Sound") ) , wxEmptyString, wxITEM_CHECK );
	m_calcMenu->Append( m_soundMenuItem );
	
	wxMenuItem* m_separator3;
	m_separator3 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_pauseMenuItem;
	m_pauseMenuItem = new wxMenuItem( m_calcMenu, wxID_ANY, wxString( wxT("Pause Emulation") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_pauseMenuItem );
	
	m_speedMenu = new wxMenu();
	m_calcMenu->Append( -1, wxT("Speed"), m_speedMenu );
	
	wxMenuItem* m_separator4;
	m_separator4 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_connectMenuItem;
	m_connectMenuItem = new wxMenuItem( m_calcMenu, wxID_ANY, wxString( wxT("Connect To...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_connectMenuItem );
	
	wxMenuItem* m_separator6;
	m_separator6 = m_calcMenu->AppendSeparator();
	
	wxMenuItem* m_varsMenuItem;
	m_varsMenuItem = new wxMenuItem( m_calcMenu, wxID_ANY, wxString( wxT("Variables...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_varsMenuItem );
	
	wxMenuItem* m_optionsMenuItem;
	m_optionsMenuItem = new wxMenuItem( m_calcMenu, wxID_ANY, wxString( wxT("Options...") ) , wxEmptyString, wxITEM_NORMAL );
	m_calcMenu->Append( m_optionsMenuItem );
	
	m_menubar->Append( m_calcMenu, wxT("Calculator") );
	
	m_debugMenu = new wxMenu();
	wxMenuItem* m_resetMenuItem;
	m_resetMenuItem = new wxMenuItem( m_debugMenu, wxID_ANY, wxString( wxT("Reset") ) , wxEmptyString, wxITEM_NORMAL );
	m_debugMenu->Append( m_resetMenuItem );
	
	wxMenuItem* m_debugMenuItem;
	m_debugMenuItem = new wxMenuItem( m_debugMenu, wxID_ANY, wxString( wxT("Open Debugger...") ) , wxEmptyString, wxITEM_NORMAL );
	m_debugMenu->Append( m_debugMenuItem );
	
	m_menubar->Append( m_debugMenu, wxT("Debug") );
	
	m_helpMenu = new wxMenu();
	wxMenuItem* m_websiteMenuItem;
	m_websiteMenuItem = new wxMenuItem( m_helpMenu, wxID_ANY, wxString( wxT("Website") ) , wxEmptyString, wxITEM_NORMAL );
	m_helpMenu->Append( m_websiteMenuItem );
	
	wxMenuItem* m_aboutMenuItem;
	m_aboutMenuItem = new wxMenuItem( m_helpMenu, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_helpMenu->Append( m_aboutMenuItem );
	
	m_menubar->Append( m_helpMenu, wxT("Help") );
	
	this->SetMenuBar( m_menubar );
	
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
}

wxWabbitemu::~wxWabbitemu()
{
}

void wxWabbitemu::gui_draw(int slot)
{
	gslot = slot;
	this->Update();

	//UpdateDebugTrack();
	if (calcs[gslot].gif_disp_state != GDS_IDLE) {
		static int skip = 0;
		if (skip == 0) {
			gif_anim_advance = TRUE;
			InvalidateRect(calcs[gslot].hwndFrame, NULL, FALSE);
		}
		skip = (skip + 1) % 4;
	}
}
