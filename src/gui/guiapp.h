#include "stdafx.h"
#include "gui.h"
#include "calc.h"
#include <wx/config.h>

class WabbitemuApp: public wxApp
{
private:
	virtual bool OnInit();
	virtual int OnExit();
	void OnTimer(wxTimerEvent& event);
	void getTimer(int slot);
	void LoadSettings(LPCALC lpCalc);
	void SaveSettings(LPCALC lpCalc);
	wxConfigBase *settingsConfig;
	wxTimer *timer;
	unsigned GetTickCount();
public:
	static BOOL DoRomWizard();
};
