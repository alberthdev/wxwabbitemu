#ifndef ROMWIZARD_H
#define ROMWIZARD_H
#include "wizard/wizardstart.h"
#include "wizard/wizardcalctype.h"
#include "wizard/wizardos.h"
#include <wx/wx.h>
#include <wx/url.h>
#include <wx/wfstream.h>

class RomWizard : public wxWizard {
private:
	WizardStartPage *startPage;
	WizardCalcTypePage *calcTypePage;
	WizardOSPage *osPage;
	void OnFinish(wxWizardEvent &);
	void OnPageChanged(wxWizardEvent &);
	void ModelInit(LPCALC lpCalc, int model);
	BOOL ExtractBootFree(wxString &bootfreePath, int model);
	BOOL DownloadOS(wxString &osFilePath, int model, BOOL version);
protected:
	DECLARE_EVENT_TABLE()
public:
	RomWizard();
	bool Begin();
};

#endif