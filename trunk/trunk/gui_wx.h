#ifndef GUI_WX_H
#define GUI_WX_H

#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/wx.h>


void gui_frame(int);
void gui_debug(int);

class MyFrame: public wxFrame
{
public:
    MyFrame(int slot);
	wxFrame *frameMain;

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnShow(wxShowEvent& event);
	//DECLARE_EVENT_TABLE()
protected:
	
private:
	
};

#endif