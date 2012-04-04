#include "cpupane.h"
#include "alu.h"
#include "gui.h"
#include <sstream>

enum
{
	ID_Check_Halt,
	ID_Text_Freq,
	ID_Text_Bus
	
};

BEGIN_EVENT_TABLE(CPUPane, wxCollapsiblePane)
	EVT_CHECKBOX(ID_Check_Halt, CPUPane::OnCheckChanged)
	EVT_TEXT_ENTER(ID_Text_Freq, CPUPane::OnFreqTextEnter)
	EVT_TEXT_ENTER(ID_Text_Bus, CPUPane::OnBusTextEnter)
END_EVENT_TABLE()

CPUPane::CPUPane(wxWindow *parent, DebuggerWindowClass *debugWindow, LPCALC lpCalc) :
wxCollapsiblePane(parent, wxID_ANY, wxT("CPU Status"), wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE | wxCP_DEFAULT_STYLE)
{
	this->lpCalc = lpCalc;
	this->debugWindow = debugWindow;
	wxWindow *cpuPaneWindow = GetPane();

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );
	
	m_haltCheck = new wxCheckBox( cpuPaneWindow, ID_Check_Halt, wxT("Halt"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( m_haltCheck, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText23 = new wxStaticText( cpuPaneWindow, wxID_ANY, wxT("Freq"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	bSizer28->Add( m_staticText23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_freqText = new wxTextCtrl( cpuPaneWindow, ID_Text_Freq, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer28->Add( m_freqText, 0, wxALL, 5 );
	
	bSizer27->Add( bSizer28, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText24 = new wxStaticText( cpuPaneWindow, wxID_ANY, wxT("Bus"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	bSizer29->Add( m_staticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_busText = new wxTextCtrl( cpuPaneWindow, ID_Text_Bus, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer29->Add( m_busText, 0, wxALL, 5 );
	
	bSizer27->Add( bSizer29, 0, wxEXPAND, 5 );
	
	cpuPaneWindow->SetSizer( bSizer27 );
	cpuPaneWindow->Layout();
	bSizer27->Fit( cpuPaneWindow );
}

void CPUPane::DebugUpdateWindow() {
	m_haltCheck->SetValue(lpCalc->cpu.halt);
	m_freqText->SetValue(wxString::Format(_T("%.2f"), lpCalc->timer_c.freq / 1000000.0));
	m_busText->SetValue(wxString::Format(wxT("%02X"), lpCalc->cpu.bus));
}

void CPUPane::OnFreqTextEnter(wxCommandEvent &event) {
	std::stringstream ss;
	ss << m_freqText->GetValue();
	ss >> lpCalc->timer_c.freq;
	lpCalc->timer_c.freq *= 1000000;
	debugWindow->DebugUpdateWindow();
}

void CPUPane::OnBusTextEnter(wxCommandEvent &event) {
	int output;
	std::stringstream ss;
	ss << std::hex << m_busText->GetValue();
	ss >> output;
	lpCalc->cpu.bus = (unsigned char) output;
	debugWindow->DebugUpdateWindow();
}


void CPUPane::OnCheckChanged(wxCommandEvent &event) {
	lpCalc->cpu.halt = !lpCalc->cpu.halt;
	debugWindow->DebugUpdateWindow();
}