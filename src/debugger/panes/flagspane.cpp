#include "flagspane.h"
#include "alu.h"

enum
{
	ID_Radio_Z,
	ID_Radio_C,
	ID_Radio_S,
	ID_Radio_PV,
	ID_Radio_HC,
	ID_Radio_N
};

BEGIN_EVENT_TABLE(FlagsPane, wxCollapsiblePane)
	EVT_CHECKBOX(wxID_ANY, FlagsPane::OnCheckChanged)
END_EVENT_TABLE()

FlagsPane::FlagsPane(wxWindow *parent, DebuggerWindowClass *debugWindow, LPCALC lpCalc) : wxCollapsiblePane(parent, wxID_ANY, "Flags", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE | wxCP_DEFAULT_STYLE)
{
	this->lpCalc = lpCalc;
	
	wxWindow *flagsPaneWindow = GetPane();
	this->debugWindow = debugWindow;
	
	wxGridSizer* gSizer5;
	gSizer5 = new wxGridSizer( 2, 3, 0, 0 );
	
	m_zCheck = new wxCheckBox( flagsPaneWindow, ID_Radio_Z, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( m_zCheck, 0, wxALL, 5 );
	
	m_cCheck = new wxCheckBox( flagsPaneWindow, ID_Radio_C, wxT("c"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( m_cCheck, 0, wxALL, 5 );
	
	m_sCheck = new wxCheckBox( flagsPaneWindow, ID_Radio_S, wxT("s"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( m_sCheck, 0, wxALL, 5 );
	
	m_pvCheck = new wxCheckBox( flagsPaneWindow, ID_Radio_PV, wxT("p/v"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( m_pvCheck, 0, wxALL, 5 );
	
	m_hcCheck = new wxCheckBox( flagsPaneWindow, ID_Radio_HC, wxT("hc"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( m_hcCheck, 0, wxALL, 5 );
	
	m_nCheck = new wxCheckBox( flagsPaneWindow, ID_Radio_N, wxT("n"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( m_nCheck, 0, wxALL, 5 );
	
	flagsPaneWindow->SetSizer( gSizer5 );
	flagsPaneWindow->Layout();
	
	flagsPaneWindow->SetSizer(gSizer5);
	flagsPaneWindow->Layout();
	gSizer5->Fit( flagsPaneWindow );
}

void FlagsPane::DebugUpdateWindow() {
	m_zCheck->SetValue((lpCalc->cpu.f & ZERO_MASK) != 0);
	m_cCheck->SetValue((lpCalc->cpu.f & CARRY_MASK) != 0);
	m_sCheck->SetValue((lpCalc->cpu.f & SIGN_MASK) != 0);
	m_pvCheck->SetValue((lpCalc->cpu.f & PV_MASK) != 0);
	m_hcCheck->SetValue((lpCalc->cpu.f & HC_MASK) != 0);
	m_nCheck->SetValue((lpCalc->cpu.f & N_MASK) != 0);
}

void FlagsPane::OnCheckChanged(wxCommandEvent &event) {
	switch (event.GetId()) {
		case ID_Radio_Z:
			lpCalc->cpu.f ^= ZERO_MASK;
			break;
		case ID_Radio_C:
			lpCalc->cpu.f ^= CARRY_MASK;
			break;
		case ID_Radio_S:
			lpCalc->cpu.f ^= SIGN_MASK;
			break;
		case ID_Radio_PV:
			lpCalc->cpu.f ^= PV_MASK;
			break;
		case ID_Radio_HC:
			lpCalc->cpu.f ^= HC_MASK;
			break;
		case ID_Radio_N:
			lpCalc->cpu.f ^= N_MASK;
			break;
	}
	debugWindow->DebugUpdateWindow();
}