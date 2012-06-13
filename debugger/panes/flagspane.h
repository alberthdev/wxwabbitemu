#ifndef FLAGSPANE_H
#define FLAGSPANE_H

#include <wx/wx.h>
#include <wx/collpane.h>

#include "calc.h"
#include "debuggerwindowclass.h"

class FlagsPane : public wxCollapsiblePane, public DebuggerWindowClass {
private:
	LPCALC lpCalc;
	wxPanel* m_flagsPane;
	wxCheckBox* m_zCheck;
	wxCheckBox* m_cCheck;
	wxCheckBox* m_sCheck;
	wxCheckBox* m_pvCheck;
	wxCheckBox* m_hcCheck;
	wxCheckBox* m_nCheck;
	DebuggerWindowClass* debugWindow;
	
	void OnCheckChanged(wxCommandEvent &event);
protected:
	DECLARE_EVENT_TABLE()
public:
	FlagsPane(wxWindow *parent, DebuggerWindowClass *debugWindow, LPCALC lpCalc);
	void DebugUpdateWindow();
};

#endif