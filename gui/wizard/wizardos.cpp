#include "wizardos.h"
#include "gui.h"
#include "var.h"

BEGIN_EVENT_TABLE(WizardOSPage, wxWizardPage)
	EVT_FILEPICKER_CHANGED(wxID_ANY, WizardOSPage::OnFilePickerChanged)
END_EVENT_TABLE()

WizardOSPage::WizardOSPage( wxWizard *parent ) : wxWizardPage(parent)
{
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("A calculator OS file is required to emulate TI calculators."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( 350 );
	bSizer20->Add( m_staticText4, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Note that TI has discontinued the ability to automatically download OS updates. If you need a calculator OS file, please download it from their website:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( 350 );
	bSizer20->Add( m_staticText5, 0, wxALL, 5 );
	
	m_hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("Download your OS here"), wxT("https://education.ti.com/en/us/downloads-and-activities"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	bSizer20->Add( m_hyperlink1, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Select a calculator OS file:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( 400 );
	bSizer20->Add( m_staticText6, 0, wxALL, 5 );
	
	m_filePicker2 = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxSize( 200,-1 ), wxFLP_DEFAULT_STYLE );
	m_filePicker2->SetMinSize( wxSize( 250,40 ) );
	
	bSizer20->Add( m_filePicker2, 0, wxALL, 5 );
	
	this->SetSizer( bSizer20 );
	this->Layout();
}

bool WizardOSPage::UpdateOSPathState(bool error) {
	TIFILE_t *tifile;
	bool ready = false;
	wxWindow *win = FindWindowById(wxID_FORWARD, GetParent());
	wxMessageDialog *dial;
	wxString path = m_filePicker2->GetPath();
	
	if (!path.IsEmpty()) {
		tifile = importvar(path.c_str(), TRUE);
		if (tifile == NULL || !((tifile->type == TifileVarType_t::FLASH_TYPE) && (tifile->flash) && (tifile->flash->type == FLASH_TYPE_OS))) {
			if (error) {
				dial = new wxMessageDialog(NULL, wxT("Invalid OS file specified! Please select a valid OS file, and try again."),
					wxT("Error loading OS file"),
					wxOK | wxICON_ERROR);
				dial->ShowModal();
			}
			ready = false;
		} else {
			ready = true;
		}
		FreeTiFile(tifile);
	} else {
		ready = false;
	}
	
	win->Enable(ready);
	return ready;
}

void WizardOSPage::OnFilePickerChanged(wxFileDirPickerEvent &event) {
	this->UpdateOSPathState(true);
}

wxWizardPage * WizardOSPage::GetPrev() const {
	return prev;
}

wxWizardPage * WizardOSPage::GetNext() const {
	if (creatingROM) {
		return NULL;
	}
	return next;
}
