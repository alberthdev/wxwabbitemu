#ifndef REGPANE_H
#define REGPANE_H

#include <wx/wx.h>
#include <wx/collpane.h>

#include "calc.h"
#include "debuggerwindowclass.h"

class RegPane : public wxCollapsiblePane, public DebuggerWindowClass {
private:
	wxTextCtrl* m_afText;
	wxTextCtrl* m_afpText;
	wxTextCtrl* m_bcText;
	wxTextCtrl* m_bcpText;
	wxTextCtrl* m_deText;
	wxTextCtrl* m_depText;
	wxTextCtrl* m_hlText;
	wxTextCtrl* m_hlpText;
	wxTextCtrl* m_ixText;
	wxTextCtrl* m_iyText;
	wxTextCtrl* m_pcText;
	wxTextCtrl* m_spText;
	LPCALC lpCalc;
	void OnTextEntered(wxCommandEvent &event);
protected:
	DECLARE_EVENT_TABLE()
public:
	RegPane(wxWindow *parent, LPCALC lpCalc);
	void DebugUpdateWindow();
};

#endif