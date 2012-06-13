#ifndef DISASSEMBLYVIEW_H
#define DISASSEMBLYVIEW_H
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "debuggerwindowclass.h"
#include "disassemble.h"

class DisassemblyView: public wxListCtrl, DebuggerWindowClass {
public:
	DisassemblyView(wxWindow *parent, LPCALC lpCalc, ViewType type);
	wxString OnGetItemText(long item, long column) const;
	wxListItemAttr * OnGetItemAttr(long item) const;
	void GotoAddress(waddr_t waddr);
	ViewType viewType;
	waddr_t& MapIndexToAddress(int index);
	int MapAddressToIndex(waddr_t &address);
	
	void DebugUpdateWindow();
private:
	LPCALC lpCalc;
	void sprint_data(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const;
	void sprint_addr(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const;
	void sprint_command(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const;
	void sprint_size(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const;
	void sprint_clocks(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const;
	int FindLastItem();
	Z80_info_t *zinf;
};
#endif