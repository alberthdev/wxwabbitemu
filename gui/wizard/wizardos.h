#ifndef WIZARDOS_H
#define WIZARDOS_H

#include <wx/wx.h>
#include <wx/wizard.h>
#include <wx/filepicker.h>
#include <wx/hyperlink.h>

class WizardOSPage : public wxWizardPage
{
private:
	wxStaticText* m_staticText4;
	wxStaticText* m_staticText5;
	wxStaticText* m_staticText6;
	wxHyperlinkCtrl* m_hyperlink1;
	void OnFilePickerChanged(wxFileDirPickerEvent &);
protected:
	DECLARE_EVENT_TABLE()
public:
	WizardOSPage( wxWizard* parent );
	virtual wxWizardPage *GetNext() const;
	virtual wxWizardPage *GetPrev() const;
	bool UpdateWizard(bool error = false);
	//TOOD: make getters and setters
	wxWizardPage *prev;
	wxWizardPage *next;
	int model;
	bool creatingROM;
	wxFilePickerCtrl* m_filePicker2;
};


#endif
