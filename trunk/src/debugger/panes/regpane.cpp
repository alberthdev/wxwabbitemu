#include "regpane.h"
#include <sstream>

enum
{
	ID_Text_AF,
	ID_Text_AFP,
	ID_Text_BC,
	ID_Text_BCP,
	ID_Text_DE,
	ID_Text_DEP,
	ID_Text_HL,
	ID_Text_HLP,
	ID_Text_IX,
	ID_Text_IY,
	ID_Text_SP,
	ID_Text_PC,
};

BEGIN_EVENT_TABLE(RegPane, wxCollapsiblePane)
	EVT_TEXT_ENTER(ID_Text_AF, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_AFP, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_BC, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_BCP, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_DE, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_DEP, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_HL, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_HLP, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_IX, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_IY, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_SP, RegPane::OnTextEntered)
	EVT_TEXT_ENTER(ID_Text_PC, RegPane::OnTextEntered)
END_EVENT_TABLE()

RegPane::RegPane(wxWindow *parent, LPCALC lpCalc) : wxCollapsiblePane(parent, wxID_ANY, "Registers", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE | wxCP_DEFAULT_STYLE)
{
	this->lpCalc = lpCalc;
	
	wxGridSizer* regPaneSizer;
	regPaneSizer = new wxGridSizer( 6, 2, 0, 0 );
	
	wxWindow *regPaneWindow = GetPane();
	
	wxBoxSizer* afSizer;
	afSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText9 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("AF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	afSizer->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_afText = new wxTextCtrl( regPaneWindow, ID_Text_AF, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_afText->SetMaxLength( 4 ); 
	afSizer->Add( m_afText, 0, wxALL, 5 );
	regPaneSizer->Add( afSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* afpSizer;
	afpSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText91 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("AF'"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	afpSizer->Add( m_staticText91, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_afpText = new wxTextCtrl( regPaneWindow, ID_Text_AFP, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	afpSizer->Add( m_afpText, 0, wxALL, 5 );
	regPaneSizer->Add( afpSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* bcSizer;
	bcSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText92 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("BC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText92->Wrap( -1 );
	bcSizer->Add( m_staticText92, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_bcText = new wxTextCtrl( regPaneWindow, ID_Text_BC, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_bcText->SetMaxLength( 4 ); 
	bcSizer->Add( m_bcText, 0, wxALL, 5 );
	regPaneSizer->Add( bcSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* bcpSizer;
	bcpSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText93 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("BC'"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText93->Wrap( -1 );
	bcpSizer->Add( m_staticText93, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_bcpText = new wxTextCtrl( regPaneWindow, ID_Text_BCP, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_bcpText->SetMaxLength( 4 ); 
	bcpSizer->Add( m_bcpText, 0, wxALL, 5 );
	regPaneSizer->Add( bcpSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* deSizer;
	deSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText94 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("DE"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText94->Wrap( -1 );
	deSizer->Add( m_staticText94, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_deText = new wxTextCtrl( regPaneWindow, ID_Text_DE, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_deText->SetMaxLength( 4 ); 
	deSizer->Add( m_deText, 0, wxALL, 5 );
	regPaneSizer->Add( deSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* depSizer;
	depSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText95 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("DE'"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText95->Wrap( -1 );
	depSizer->Add( m_staticText95, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_depText = new wxTextCtrl( regPaneWindow, ID_Text_DEP, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_depText->SetMaxLength( 4 ); 
	depSizer->Add( m_depText, 0, wxALL, 5 );
	regPaneSizer->Add( depSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* hlSizer;
	hlSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText96 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("HL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText96->Wrap( -1 );
	hlSizer->Add( m_staticText96, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_hlText = new wxTextCtrl( regPaneWindow, ID_Text_HL, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_hlText->SetMaxLength( 4 ); 
	hlSizer->Add( m_hlText, 0, wxALL, 5 );
	regPaneSizer->Add( hlSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* hlpSizer;
	hlpSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText97 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("HL'"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText97->Wrap( -1 );
	hlpSizer->Add( m_staticText97, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_hlpText = new wxTextCtrl( regPaneWindow, ID_Text_HLP, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_hlpText->SetMaxLength( 4 ); 
	hlpSizer->Add( m_hlpText, 0, wxALL, 5 );
	regPaneSizer->Add( hlpSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* ixSizer;
	ixSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText98 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("IX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText98->Wrap( -1 );
	ixSizer->Add( m_staticText98, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_ixText = new wxTextCtrl( regPaneWindow, ID_Text_IX, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_ixText->SetMaxLength( 4 ); 
	ixSizer->Add( m_ixText, 0, wxALL, 5 );
	regPaneSizer->Add( ixSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* iySizer;
	iySizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText99 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("IY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText99->Wrap( -1 );
	iySizer->Add( m_staticText99, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_iyText = new wxTextCtrl( regPaneWindow, ID_Text_IY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_iyText->SetMaxLength( 4 ); 
	iySizer->Add( m_iyText, 0, wxALL, 5 );
	regPaneSizer->Add( iySizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* spSizer;
	spSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText910 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("SP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText910->Wrap( -1 );
	spSizer->Add( m_staticText910, 0, wxALIGN_CENTER_VERTICAL||wxALL, 5 );
	m_spText = new wxTextCtrl( regPaneWindow, ID_Text_SP, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_spText->SetMaxLength( 4 ); 
	spSizer->Add( m_spText, 0, wxALL, 5 );
	regPaneSizer->Add( spSizer, 1, wxALIGN_RIGHT, 5 );
	wxBoxSizer* pcSizer;
	pcSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *m_staticText911 = new wxStaticText( regPaneWindow, wxID_ANY, wxT("PC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText911->Wrap( -1 );
	pcSizer->Add( m_staticText911, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	m_pcText = new wxTextCtrl( regPaneWindow, ID_Text_PC, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER  );
	m_pcText->SetMaxLength( 4 ); 
	pcSizer->Add( m_pcText, 0, wxALL, 5 );
	regPaneSizer->Add( pcSizer, 1, wxALIGN_RIGHT, 5 );	
	
	regPaneWindow->SetSizer(regPaneSizer);
	regPaneWindow->Layout();
	this->Expand();
}

void RegPane::OnTextEntered(wxCommandEvent &event)
{
	std::stringstream ss;
	switch (event.GetId()) {
		case ID_Text_AF:
			ss << std::hex << m_afText->GetValue(); 
			ss >> lpCalc->cpu.af;
			break;
		case ID_Text_AFP:
			ss << std::hex << m_afpText->GetValue(); 
			ss >> lpCalc->cpu.afp;
			break;
		case ID_Text_BC:
			ss << std::hex << m_bcText->GetValue(); 
			ss >> lpCalc->cpu.bc;
			break;
		case ID_Text_BCP:
			ss << std::hex << m_bcpText->GetValue(); 
			ss >> lpCalc->cpu.bcp;
			break;
		case ID_Text_DE:
			ss << std::hex << m_deText->GetValue(); 
			ss >> lpCalc->cpu.de;
			break;
		case ID_Text_DEP:
			ss << std::hex << m_depText->GetValue(); 
			ss >> lpCalc->cpu.dep;
			break;
		case ID_Text_HL:
			ss << std::hex << m_hlText->GetValue(); 
			ss >> lpCalc->cpu.hl;
			break;
		case ID_Text_HLP:
			ss << std::hex << m_hlpText->GetValue(); 
			ss >> lpCalc->cpu.hlp;
			break;
		case ID_Text_IX:
			ss << std::hex << m_ixText->GetValue(); 
			ss >> lpCalc->cpu.ix;
			break;
		case ID_Text_IY:
			ss << std::hex << m_iyText->GetValue(); 
			ss >> lpCalc->cpu.iy;
			break;
		case ID_Text_PC:
			ss << std::hex << m_pcText->GetValue(); 
			ss >> lpCalc->cpu.pc;
			break;
		case ID_Text_SP:
			ss << std::hex << m_spText->GetValue(); 
			ss >> lpCalc->cpu.sp;
			break;
	}
}

void RegPane::DebugUpdateWindow() {
	m_afText->SetValue(wxString::Format("%04X", lpCalc->cpu.af));
	m_afpText->SetValue(wxString::Format("%04X", lpCalc->cpu.afp));
	m_bcText->SetValue(wxString::Format("%04X", lpCalc->cpu.bc));
	m_bcpText->SetValue(wxString::Format("%04X", lpCalc->cpu.bcp));
	m_deText->SetValue(wxString::Format("%04X", lpCalc->cpu.de));
	m_depText->SetValue(wxString::Format("%04X", lpCalc->cpu.dep));
	m_hlText->SetValue(wxString::Format("%04X", lpCalc->cpu.hl));
	m_hlpText->SetValue(wxString::Format("%04X", lpCalc->cpu.hlp));
	m_ixText->SetValue(wxString::Format("%04X", lpCalc->cpu.ix));
	m_iyText->SetValue(wxString::Format("%04X", lpCalc->cpu.iy));
	m_spText->SetValue(wxString::Format("%04X", lpCalc->cpu.sp));
	m_pcText->SetValue(wxString::Format("%04X", lpCalc->cpu.pc));
}