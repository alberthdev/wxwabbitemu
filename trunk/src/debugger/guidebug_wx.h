#include "gui_wx.h"
#include "disassemblyview.h"
#include "debuggerwindowclass.h"

#include "panes/regpane.h"
#include "panes/flagspane.h"
#include "panes/cpupane.h"

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/collpane.h>
#define wxDEBUGGERID 1

/* TODO:
 * [21:07:24] <Buckeye> alberthro at this point you need to do 4 things
 * [21:07:30] <Buckeye> in this order
 * [21:08:25] <Buckeye> 4. make a memory view
 * [21:08:49] <Buckeye> at that point your debugger is adequate enough to solve 70% of problems
 * 
 * We'll get there... eventually! :)
 */

class WabbitemuDebugger: public wxFrame, public DebuggerWindowClass
{
public:
	WabbitemuDebugger(WabbitemuFrame *windowMain, LPCALC lpCalc);
	void DebugUpdateWindow();
	DECLARE_EVENT_TABLE()
protected:
	wxSplitterWindow* m_splitter1;
	wxScrolledWindow* m_scrolledWindow1;
	DisassemblyView* m_disasmView;
	wxScrolledWindow* m_scrolledWindow6;
	wxPanel* m_disasmGotoPanel;
	wxButton* m_disasmGotoButton;
	wxBoxSizer* bSizer3;
	wxStaticText* m_staticText22;
	wxTextCtrl* m_disasmGotoText;
	wxNotebook* m_notebook1;
	wxNotebook* m_notebook2;
	wxScrolledWindow* m_scrolledWindow4;
	wxPanel* m_panel1;
	wxPanel* m_panel2;
	wxPanel* m_panel4;
	wxScrolledWindow* m_scrolledWindow14;
	wxToolBar* m_toolBar1;
	RegPane *regPane;
	FlagsPane *flagsPane;
	CPUPane *cpuPane;
	wxCollapsiblePane* interruptPane;
	wxCollapsiblePane* keyboardPane;
	wxCollapsiblePane* displayPane;
	
private:
	LPCALC lpCalc;
	WabbitemuFrame *wabbitemuFrame;
	
	
	void OnClose(wxCloseEvent& event);
	void OnToolbarRun(wxCommandEvent & WXUNUSED(event));
	void OnToolbarBreak(wxCommandEvent & WXUNUSED(event));
	void OnToolbarMemBreak(wxCommandEvent & WXUNUSED(event));
	void OnToolbarStep(wxCommandEvent & WXUNUSED(event));
	void OnToolbarStepOver(wxCommandEvent & WXUNUSED(event));
	void OnToolbarGoto(wxCommandEvent & WXUNUSED(event));
	
	void OnDisasmGoto(wxCommandEvent & WXUNUSED(event));
	void OnDisasmGotoEnter(wxCommandEvent & WXUNUSED(event));
	
	void OnCollapsiblePaneChanged(wxCollapsiblePaneEvent &event);
};
