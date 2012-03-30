#ifndef WIZARDCALCTYPE_H
#define WIZARDCALCTYPE_H

#include <wx/wx.h>
#include <wx/wizard.h>
#include "calc.h"

class WizardCalcTypePage : public wxWizardPage
{
private:
	LPCALC lpCalc;
protected:
	wxStaticText* m_staticText2;
	wxRadioButton* m_calc73;
	wxRadioButton* m_calc82;
	wxRadioButton* m_calc83;
	wxRadioButton* m_calc83p;
	wxRadioButton* m_calc83pse;
	wxRadioButton* m_calc84p;
	wxRadioButton* m_calc84pse;
	wxRadioButton* m_calc85;
	wxRadioButton* m_calc86;
public:
	WizardCalcTypePage( wxWizard* parent );
	virtual wxWizardPage *GetNext() const;
	virtual wxWizardPage *GetPrev() const;
	int GetModel() const;
	void EnableRadios(bool enableRadios) const;
	wxWizardPage *prev;
	wxWizardPage *next;
};

#endif
