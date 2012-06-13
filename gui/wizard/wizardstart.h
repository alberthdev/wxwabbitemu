#ifndef WIZARDSTART_H
#define WIZARDSTART_H

#include <wx/wx.h>
#include <wx/wizard.h>
#include <wx/filepicker.h>
#include "calc.h"

class WizardStartPage : public wxWizardPage
{
private:
	void OnRadioSelected(wxCommandEvent &);
	void OnFileChanged(wxFileDirPickerEvent &);
	void OnFinished(wxWizardEvent &);
protected:
	wxStaticText* m_staticText1;
	DECLARE_EVENT_TABLE()
public:
	WizardStartPage( wxWizard* parent );
	virtual wxWizardPage *GetNext() const;
	virtual wxWizardPage *GetPrev() const;
	//TODO: make getters and setters
	wxRadioButton* m_browseRadio;	
	wxRadioButton* m_createRadio;
	wxRadioButton* m_copyRadio;
	wxFilePickerCtrl* m_filePicker1;	
	wxWizardPage *next;
	LPCALC lpCalc;
};

#endif
