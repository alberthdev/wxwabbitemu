#include "wizardos.h"
#include "gui_wx.h"

BEGIN_EVENT_TABLE(WizardOSPage, wxWizardPage)
	EVT_RADIOBUTTON(wxID_ANY, WizardOSPage::OnRadioSelected)
END_EVENT_TABLE()

WizardOSPage::WizardOSPage( wxWizard *parent ) : wxWizardPage(parent)
{
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("A calculator OS file is required in addition to a ROM image to emulate TI calculators. How do you want to obtain an OS for Wabbitemu?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( 400 );
	bSizer20->Add( m_staticText4, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer20->Add( m_staticText5, 0, wxALL, 5 );
	
	m_downloadOS = new wxRadioButton( this, wxID_ANY, wxT("Download OS files from TI's website"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer20->Add( m_downloadOS, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer21->SetMinSize( wxSize( -1,30 ) ); 
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("TI-OS Version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer21->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxArrayString m_choice1Choices;
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer21->Add( m_choice1, 0, wxALL, 5 );
	
	bSizer20->Add( bSizer21, 0, 0, 5 );
	
	m_hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, wxT("By downloading you agree to TI's terms"), wxT("http://education.ti.com/educationportal/downloadcenter/Eula.do?website=US&tabId=1&appId=317"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	bSizer20->Add( m_hyperlink1, 0, wxALL, 5 );
	
	m_browseOS = new wxRadioButton( this, wxID_ANY, wxT("Browse for OS files on my computer"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_browseOS, 0, wxALL, 5 );
	
	m_filePicker2 = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxSize( 200,-1 ), wxFLP_DEFAULT_STYLE );
	m_filePicker2->SetMinSize( wxSize( 250,40 ) );
	
	bSizer20->Add( m_filePicker2, 0, wxALL, 5 );
	
	this->SetSizer( bSizer20 );
	this->Layout();
}

void WizardOSPage::OnRadioSelected(wxCommandEvent &event) {
	wxWindow *win = FindWindowById(wxID_FORWARD, GetParent());
	if (creatingROM) {
		win->SetLabel("Finish");
	} else {
		win->SetLabel("Next >");
	}
	if (m_browseOS->GetValue()) {
		wxString path = m_filePicker2->GetPath();
		char *normalPath = wxStringToChar(path);
		TIFILE_t *tifile =  newimportvar(normalPath, TRUE);
		delete normalPath;
		if (tifile == NULL || tifile->type != FLASH_TYPE) {
			//technically this allows apps as well. dont care
			win->Enable(false);
		}
		FreeTiFile(tifile);
		return;
	} else {
		win->Enable(true);
	}
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