///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wxWabbitemu__
#define __wxWabbitemu__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include "core.h"
#include "calc.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class wxWabbitemu
///////////////////////////////////////////////////////////////////////////////
class wxWabbitemu : public wxFrame 
{
	private:
		
	protected:
		wxMenuBar* m_menubar;
		wxMenu* m_fileMenu;
		wxMenu* m_editMenu;
		wxMenu* m_calcMenu;
		wxMenu* m_speedMenu;
		wxMenu* m_debugMenu;
		wxMenu* m_helpMenu;
		wxStatusBar* m_statusBar1;
	
	public:
		wxWabbitemu( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Wabbitemu"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxTAB_TRAVERSAL, const wxString& name = wxT("Wabbitemu") );
		~wxWabbitemu();
		void gui_draw(int);
	
};

#endif //__noname__
