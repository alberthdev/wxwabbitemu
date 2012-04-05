#include "guiapp.h"
#include "gui.h"
#include "wizard/romwizard.h"

WabbitemuFrame *frames[MAX_CALCS];

BOOL WabbitemuApp::DoRomWizard() {
	RomWizard wizard;	
	bool success = wizard.Begin();
	return success;
}

void WabbitemuApp::LoadSettings(LPCALC lpCalc)
{
	settingsConfig = new wxConfig(wxT("Wabbitemu"));
	wxString tempString;
	settingsConfig->Read(wxT("/rom_path"), &tempString, wxEmptyString);
	_tcscpy(lpCalc->rom_path, tempString.c_str());
	settingsConfig->Read(wxT("/SkinEnabled"), &lpCalc->SkinEnabled, FALSE);
}

bool WabbitemuApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	memset(frames, 0, sizeof(frames));
	LPCALC lpCalc = calc_slot_new();
	LoadSettings(lpCalc);
	
	WabbitemuFrame *frame;
	int result = rom_load(lpCalc, lpCalc->rom_path);
	if (result == TRUE) {
		frame = gui_frame(lpCalc);
	} else {
		calc_slot_free(lpCalc);
		bool success = DoRomWizard();
		if (!success) {
			return FALSE;
		}
	}
	timer = new wxTimer();
	timer->Connect(wxEVT_TIMER, (wxObjectEventFunction) &WabbitemuApp::OnTimer);
	timer->Start(TPF, false);
	return TRUE;
}

void WabbitemuApp::SaveSettings(LPCALC lpCalc) {
#ifdef _UNICODE
	wxString rom_path(lpCalc->rom_path, wxConvUTF8);
#else
	wxString rom_path(lpCalc->rom_path);
#endif
	settingsConfig->Write(wxT("rom_path"), rom_path);
	settingsConfig->Write(wxT("SkinEnabled"), lpCalc->SkinEnabled);
	settingsConfig->Flush();
}

int WabbitemuApp::OnExit() {
	SaveSettings(&calcs[0]);
	return 0;
}


unsigned WabbitemuApp::GetTickCount()
{
		struct timeval tv;
		if(gettimeofday(&tv, NULL) != 0)
			return 0;

		return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


void WabbitemuApp::OnTimer(wxTimerEvent& event) {
	static int difference;
	static unsigned prevTimer;
	unsigned dwTimer = GetTickCount();
	
	// How different the timer is from where it should be
	// guard from erroneous timer calls with an upper bound
	// that's the limit of time it will take before the
	// calc gives up and claims it lost time
	difference += ((dwTimer - prevTimer) & 0x003F) - TPF;
	prevTimer = dwTimer;

	// Are we greater than Ticks Per Frame that would call for
	// a frame skip?
	if (difference > -TPF) {
		calc_run_all();
		while (difference >= TPF) {
			calc_run_all();
			difference -= TPF;
		}

		int i;
		for (i = 0; i < MAX_CALCS; i++) {
			if (calcs[i].active) {
				frames[i]->gui_draw();
			}
		}
	// Frame skip if we're too far ahead.
	} else {
		difference += TPF;
	}
}