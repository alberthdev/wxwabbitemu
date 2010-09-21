#ifndef GUI_WX_H
#define GUI_WX_H

#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/dnd.h>
#include <sys/time.h>
#if (wxUSE_UNICODE)
#include <wx/encconv.h> 
#endif

#if (wxUSE_UNICODE)
#include <wx/encconv.h>
#endif


int gui_draw(int);
int gui_frame(int);
int gui_debug(int);
int gui_frame_update(int);
char* wxStringToChar(wxString);

class MyFrame: public wxFrame
{
public:
    MyFrame(int slot);
	wxFrame *frameMain;

	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void SetSpeed(int speed);
	//void OnShow(wxShowEvent& event);
	//DECLARE_EVENT_TABLE()
protected:
	
private:
	//menu items
	void OnFileQuit(wxCommandEvent& event);
	void OnFileClose(wxCommandEvent& event);
	void OnFileSave(wxCommandEvent& event);
	void OnFileNew(wxCommandEvent& event);
	void OnFileOpen(wxCommandEvent& event);
    void OnHelpAbout(wxCommandEvent& event);
	void OnCalcSkin(wxCommandEvent& event);
	
	void OnSize(wxSizeEvent& event);
	void FinalizeButtons();
};

#endif
