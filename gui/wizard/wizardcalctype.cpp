#include "wizardcalctype.h"

WizardCalcTypePage::WizardCalcTypePage( wxWizard* parent ) : wxWizardPage(parent)
{
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("What type of calculator would you like to emulate?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer19->Add( m_staticText2, 0, wxALL, 5 );
	
	wxGridSizer* gSizer6;
	gSizer6 = new wxGridSizer( 3, 3, 0, 0 );
	
	m_calc73 = new wxRadioButton( this, wxID_ANY, wxT("TI-73"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	gSizer6->Add( m_calc73, 0, wxALL, 5 );
	
	m_calc82 = new wxRadioButton( this, wxID_ANY, wxT("TI-82"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_calc82, 0, wxALL, 5 );
	
	m_calc83 = new wxRadioButton( this, wxID_ANY, wxT("TI-83"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_calc83, 0, wxALL, 5 );
	
	m_calc83p = new wxRadioButton( this, wxID_ANY, wxT("TI-83 Plus"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_calc83p, 0, wxALL, 5 );
	
	m_calc83pse = new wxRadioButton( this, wxID_ANY, wxT("TI-83 Plus SE"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_calc83pse, 0, wxALL, 5 );
	
	m_calc84p = new wxRadioButton( this, wxID_ANY, wxT("TI-84 Plus"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_calc84p, 0, wxALL, 5 );
	
	m_calc84pse = new wxRadioButton( this, wxID_ANY, wxT("TI-84 Plus SE"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_calc84pse, 0, wxALL, 5 );
	
	m_calc85 = new wxRadioButton( this, wxID_ANY, wxT("TI-85"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_calc85, 0, wxALL, 5 );
	
	m_calc86 = new wxRadioButton( this, wxID_ANY, wxT("TI-86"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_calc86, 0, wxALL, 5 );
	
	bSizer19->Add( gSizer6, 1, wxEXPAND, 5 );
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer19 );
	this->Layout();
}

int WizardCalcTypePage::GetModel() const {
	if (m_calc73->GetValue()) {
		return TI_73;
	} else if (m_calc82->GetValue()) {
		return TI_82;
	} else if (m_calc83p->GetValue()) {
		return TI_83P;
	} else if (m_calc83pse->GetValue()) {
		return TI_83PSE;
	} else if (m_calc84p->GetValue()) {
		return TI_84P;
	} else if (m_calc84pse->GetValue()) {
		return TI_84PSE;
	} else if (m_calc85->GetValue()) {
		return TI_85;
	} else if (m_calc86->GetValue()) {
		return TI_86;
	}
	return -1;
}

void WizardCalcTypePage::EnableRadios(bool enableRadios) const {
	m_calc82->Enable(enableRadios);
	m_calc83->Enable(enableRadios);
	m_calc85->Enable(enableRadios);
	m_calc86->Enable(enableRadios);
}

wxWizardPage * WizardCalcTypePage::GetPrev() const {
	return prev;
}

wxWizardPage * WizardCalcTypePage::GetNext() const {
	return next;
}
