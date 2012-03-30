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
	wxRadioButton* m_browseOS;
	wxHyperlinkCtrl* m_hyperlink1;
	void OnRadioSelected(wxCommandEvent &);
protected:
	DECLARE_EVENT_TABLE()
public:
	WizardOSPage( wxWizard* parent );
	virtual wxWizardPage *GetNext() const;
	virtual wxWizardPage *GetPrev() const;
	//TOOD: make getters and setters
	wxWizardPage *prev;
	wxWizardPage *next;
	int model;
	bool creatingROM;
	wxChoice* m_choice1;
	wxFilePickerCtrl* m_filePicker2;
	wxRadioButton* m_downloadOS;
};


#endif