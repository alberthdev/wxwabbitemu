#include "wizardstart.h"
#include "gui.h"
#include "var.h"

BEGIN_EVENT_TABLE(WizardStartPage, wxWizardPage)
	EVT_RADIOBUTTON(wxID_ANY, WizardStartPage::OnRadioSelected)
	EVT_FILEPICKER_CHANGED(wxID_ANY, WizardStartPage::OnFileChanged)
END_EVENT_TABLE()

WizardStartPage::WizardStartPage( wxWizard* parent) : wxWizardPage( parent )
{
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("This wizard will guide you through running Wabbitemu for the first time.\n\n A ROM is required to emulate TI calculators. How do you want to get a ROM image for Wabbitemu?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap(400);
	bSizer18->Add( m_staticText1, 0, wxALL, 5 );
	
	m_browseRadio = new wxRadioButton( this, wxID_ANY, wxT("Browse for a ROM image on my computer"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer18->Add( m_browseRadio, 0, wxALL, 5 );
	
	m_filePicker1 = new wxFilePickerCtrl( this, wxID_ANY, wxT("."), wxT("Browse for a ROM image"), wxT("All known files|*.rom;*.sav|ROM images|*.rom|Savestate images|*.sav"), wxDefaultPosition, wxSize( 300,-1 ), wxFLP_DEFAULT_STYLE );
	m_filePicker1->SetMinSize( wxSize( 300,30 ) );
	
	bSizer18->Add( m_filePicker1, 0, wxALL, 5 );
	
	m_copyRadio = new wxRadioButton( this, wxID_ANY, wxT("Copy a ROM image from a real calculator"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_copyRadio, 0, wxALL, 5 );
	
	m_createRadio = new wxRadioButton( this, wxID_ANY, wxT("Create a ROM image using open source software"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_createRadio, 0, wxALL, 5 );
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	// Disable ROM copy (we don't have that implemented yet)
	m_copyRadio->Enable(false);
}

void WizardStartPage::OnFileChanged(wxFileDirPickerEvent &event) {
	wxWindow *win = FindWindowById(wxID_FORWARD, GetParent());
	wxString path = m_filePicker1->GetPath();
	TIFILE_t *tifile =  newimportvar(path.c_str(), TRUE);
	if (tifile == NULL) {
		win->Enable(false);
	} else {
		win->Enable(true);
	}
	FreeTiFile(tifile);
}

void WizardStartPage::OnRadioSelected(wxCommandEvent &event) {
	wxWindow *win = FindWindowById(wxID_FORWARD, GetParent());
	if (m_browseRadio->GetValue()) {
		win->SetLabel(wxT("Finish"));
		wxString path = m_filePicker1->GetPath();
		TIFILE_t *tifile =  newimportvar(path.c_str(), TRUE);
		if (tifile == NULL) {
			win->Enable(false);
		}
		FreeTiFile(tifile);
		m_filePicker1->Enable(true);
		return;
	}
	win->SetLabel(wxT("Next >"));
	win->Enable(true);
	m_filePicker1->Enable(false);
}

wxWizardPage * WizardStartPage::GetPrev() const {
	return NULL;
}

wxWizardPage * WizardStartPage::GetNext() const {
	if (m_browseRadio->GetValue()) {
		return NULL;
	}
	return next;
}
