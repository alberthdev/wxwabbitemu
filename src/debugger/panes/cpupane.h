#ifndef CPUPANE_H
#define CPUPANE_H

#include <wx/wx.h>
#include <wx/collpane.h>

#include "calc.h"
#include "debuggerwindowclass.h"

class CPUPane : public wxCollapsiblePane, public DebuggerWindowClass {
private:
	LPCALC lpCalc;
	wxCheckBox* m_haltCheck;
	wxStaticText* m_staticText23;
	wxTextCtrl* m_freqText;
	wxStaticText* m_staticText24;
	wxTextCtrl* m_busText;
	DebuggerWindowClass *debugWindow;
		
	void OnCheckChanged(wxCommandEvent &event);
	void OnFreqTextEnter(wxCommandEvent &event);
	void OnBusTextEnter(wxCommandEvent &event);
protected:
	DECLARE_EVENT_TABLE()
public:
	CPUPane(wxWindow *parent, DebuggerWindowClass *debugWindow, LPCALC lpCalc);
	void DebugUpdateWindow();
};

#endif