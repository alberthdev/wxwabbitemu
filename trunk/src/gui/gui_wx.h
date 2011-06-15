#ifndef GUI_WX_H
#define GUI_WX_H

#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/dnd.h>
#include <wx/mstream.h>
#include <sys/time.h>
#if (wxUSE_UNICODE)
#include <wx/encconv.h> 
#endif

#if (wxUSE_UNICODE)
#include <wx/encconv.h>
#endif

#include "skins/ti83p.h"

 #define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))

 inline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length) {
   wxMemoryInputStream is(data, length);
   return wxBitmap(wxImage(is, wxBITMAP_TYPE_PNG, -1), -1);
 }


int gui_draw(int);
int gui_frame(int);
int gui_debug(int);
int gui_frame_update(int);
char* wxStringToChar(wxString);

class MyFrame: public wxFrame
{
public:
    MyFrame(int curslot);
    
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
    void OnHelpWebsite(wxCommandEvent& event);
    
	void OnCalcSkin(wxCommandEvent& event);
	void OnPauseEmulation(wxCommandEvent& event);
	
	void OnSetSpeed(wxCommandEvent& event);
	void OnSetSize(wxCommandEvent& event);
	
	void OnPaint(wxPaintEvent& event);
	int slot;
	
	void OnSize(wxSizeEvent& event);
	void OnQuit(wxCloseEvent& event);
	void FinalizeButtons();
};

#endif
