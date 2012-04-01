#ifndef GUI_WX_H
#define GUI_WX_H

#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/numdlg.h>
#include <wx/wx.h>
#include <wx/dnd.h>
#include <wx/mstream.h>
#include <wx/filename.h>
#include <wx/config.h>
#include <sys/time.h>
#if (wxUSE_UNICODE)
#include <wx/encconv.h> 
#endif

#include "guilcd_wx.h"
#include "guiskinwindow.h"
#include "guivartree.h"
#include "calc.h"

enum
{
	ID_LCD,
};

class WabbitemuFrame: public wxFrame
{
public:
    WabbitemuFrame(LPCALC);
   	wxWindow *wxLCD;
    
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void SetSpeed(int speed);
	int gui_draw();
	void gui_frame_update();
	void OnTimer(wxTimerEvent& event);
protected:
	DECLARE_EVENT_TABLE()
private:
	wxWindow *skinWindow;
	bool is_resizing;
	VarTree *varTree;
	//menu items
	void OnFileQuit(wxCommandEvent& event);
	void OnFileClose(wxCommandEvent& event);
	void OnFileSave(wxCommandEvent& event);
	void OnFileNew(wxCommandEvent& event);
	void OnFileOpen(wxCommandEvent& event);
	void OnFileGIF(wxCommandEvent& event);
    
	void OnViewSkin(wxCommandEvent& event);
	void OnViewVariables(wxCommandEvent& event);
	void OnPauseEmulation(wxCommandEvent& event);
	void OnTurnCalcOn(wxCommandEvent& event);
	
	void OnSetSpeed(wxCommandEvent& event);
	void OnSetSpeedCustom(wxCommandEvent& event);
	void OnSetSize(wxCommandEvent& event);
	
	void OnDebugReset(wxCommandEvent& event);
	void OnDebugOpen(wxCommandEvent& event);
	void OnDebugOn(wxCommandEvent& event);
	
	void OnHelpSetup(wxCommandEvent& event);
	void OnHelpWebsite(wxCommandEvent& event);
	void OnHelpAbout(wxCommandEvent& event);
	
	void OnPaint(wxPaintEvent& event);
	// Resize
	void OnResize(wxSizeEvent& event);
	LPCALC lpCalc;
	
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnLeftButtonUp(wxMouseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnQuit(wxCloseEvent& event);
	void FinalizeButtons();
};
char* wxStringToChar(wxString);
int SetGIFName();
void gui_debug(LPCALC lpCalc);
WabbitemuFrame* gui_frame(LPCALC lpCalc);
#endif
