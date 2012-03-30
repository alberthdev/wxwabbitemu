#include "guidebug_wx.h"
#include "disassemblyview.h"
#include <sstream>

enum
{
	ID_Toolbar_Run,
	ID_Toolbar_Break,
	ID_Toolbar_Watch_Break,
	ID_Toolbar_Step,
	ID_Toolbar_StepOver,
	ID_Toolbar_Goto,
	
	ID_Disasm_GotoButton,
	ID_Disasm_GotoText,
	ID_Mem_GotoButton,
	ID_Mem_GotoText,
};

BEGIN_EVENT_TABLE(WabbitemuDebugger, wxFrame)
	EVT_CLOSE(WabbitemuDebugger::OnClose)
	
	EVT_TOOL(ID_Toolbar_Run, WabbitemuDebugger::OnToolbarRun)
	EVT_TOOL(ID_Toolbar_Break, WabbitemuDebugger::OnToolbarBreak)
	EVT_TOOL(ID_Toolbar_Watch_Break, WabbitemuDebugger::OnToolbarMemBreak)
	EVT_TOOL(ID_Toolbar_Step, WabbitemuDebugger::OnToolbarStep)
	EVT_TOOL(ID_Toolbar_StepOver, WabbitemuDebugger::OnToolbarStepOver)
	EVT_TOOL(ID_Toolbar_Goto, WabbitemuDebugger::OnToolbarGoto)
	
	EVT_BUTTON(ID_Disasm_GotoButton, WabbitemuDebugger::OnDisasmGoto)
	EVT_TEXT_ENTER(ID_Disasm_GotoText, WabbitemuDebugger::OnDisasmGotoEnter)
END_EVENT_TABLE()

WabbitemuDebugger::WabbitemuDebugger(WabbitemuFrame *frame, LPCALC lpCalc) :
	wxFrame(frame, wxDEBUGGERID, wxT("Debugger"), wxDefaultPosition, wxSize(800, 600))
{
	this->wabbitemuFrame = frame;
	this->lpCalc = lpCalc;
	
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* fgSizer2;
	fgSizer2 = new wxBoxSizer(wxHORIZONTAL);
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_scrolledWindow1 = new wxScrolledWindow( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	m_disasmGotoPanel = new wxPanel( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText22 = new wxStaticText( m_disasmGotoPanel, wxID_ANY, wxT("Goto Address"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	bSizer25->Add( m_staticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_disasmGotoText = new wxTextCtrl( m_disasmGotoPanel, ID_Disasm_GotoText, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), wxTE_PROCESS_ENTER );
	bSizer25->Add( m_disasmGotoText, 0, wxALL, 5 );
	
	m_disasmGotoButton = new wxButton( m_disasmGotoPanel, ID_Disasm_GotoButton, wxT("Go"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer25->Add( m_disasmGotoButton, 0, wxALL, 5 );
	
	m_disasmGotoPanel->SetSizer( bSizer25 );
	m_disasmGotoPanel->Layout();
	bSizer25->Fit( m_disasmGotoPanel );
	bSizer3->Add( m_disasmGotoPanel, 0, wxEXPAND | wxALL, 0 );
	
	m_notebook2 = new wxNotebook( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_disasmView = new DisassemblyView( m_notebook2, lpCalc, REGULAR);
	m_notebook2->AddPage( m_disasmView, wxT("Disassembly"), true );
	bSizer3->Add( m_notebook2, 1, wxEXPAND | wxALL, 0 );
	
	m_scrolledWindow1->SetSizer( bSizer3 );
	m_scrolledWindow1->Layout();
	bSizer3->Fit( m_scrolledWindow1 );
	m_scrolledWindow6 = new wxScrolledWindow( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow6->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook1 = new wxNotebook( m_scrolledWindow6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook1->AddPage( m_panel1, wxT("Page 1"), true );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook1->AddPage( m_panel2, wxT("Page 2"), true );
	bSizer5->Add( m_notebook1, 1, wxEXPAND | wxALL, 0 );	
	
	m_scrolledWindow6->SetSizer( bSizer5 );
	m_scrolledWindow6->Layout();
	bSizer5->Fit( m_scrolledWindow6 );
	m_splitter1->SplitHorizontally( m_scrolledWindow1, m_scrolledWindow6, 0);
	fgSizer2->Add( m_splitter1, 1, wxALL|wxEXPAND, 5 );
	
	bSizer14->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxTAB_TRAVERSAL | wxALL);
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	//m_scrolledWindow14 = new wxScrolledWindow( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	//m_scrolledWindow14->SetScrollRate( 5, 5 );
	regPane = new RegPane(m_panel4, lpCalc);
	bSizer11->Add( regPane, 0, wxEXPAND, 5 );
	
	/*flagsPane = new wxCollapsiblePane(m_panel4, wxID_ANY, "Flags", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE | wxCP_DEFAULT_STYLE);
	cpuPane = new wxCollapsiblePane(m_panel4, wxID_ANY, "CPU Status", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE | wxCP_DEFAULT_STYLE);
	interruptPane = new wxCollapsiblePane(m_panel4, wxID_ANY, "Interrupts", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE | wxCP_DEFAULT_STYLE);
	keyboardPane = new wxCollapsiblePane(m_panel4, wxID_ANY, "Keyboard", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE | wxCP_DEFAULT_STYLE);
	displayPane = new wxCollapsiblePane(m_panel4, wxID_ANY, "Display", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE | wxCP_DEFAULT_STYLE);
	
	bSizer11->Add( flagsPane, 0, wxALL|wxEXPAND, 5 );
	bSizer11->Add( cpuPane, 0, wxALL|wxEXPAND, 5 );
	bSizer11->Add( interruptPane, 0, wxALL|wxEXPAND, 5 );
	bSizer11->Add( keyboardPane, 0, wxALL|wxEXPAND, 5 );
	bSizer11->Add( displayPane, 0, wxALL|wxEXPAND, 5 );*/

	
	m_panel4->SetSizer( bSizer11 );
	m_panel4->Layout();
	
	fgSizer2->Add( m_panel4, 0, wxALL|wxEXPAND, 0 );
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL | wxTB_TEXT | wxTB_HORZ_LAYOUT, wxID_ANY ); 
	//TODO: convert these to use internal resources
	m_toolBar1->AddTool( ID_Toolbar_Run, wxT("Run"), wxBitmap( wxT("./res/run.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Run the calculator"), wxEmptyString );
	m_toolBar1->AddTool( ID_Toolbar_Break, wxT("Toggle Breakpoint"), wxBitmap( wxT("./res/break.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Toggle execution breakpoint"), wxEmptyString );
	m_toolBar1->AddTool( ID_Toolbar_Watch_Break, wxT("Toggle watchpoint"), wxBitmap( wxT("./res/membreak.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Toggle a memory breakpoint"), wxEmptyString );
	m_toolBar1->AddTool( ID_Toolbar_Step, wxT("Step"), wxBitmap( wxT("./res/step.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Run a single command"), wxEmptyString );
	m_toolBar1->AddTool( ID_Toolbar_StepOver, wxT("Step Over"), wxBitmap( wxT("./res/stepover.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Run a single line"), wxEmptyString );
	m_toolBar1->AddTool( ID_Toolbar_Goto, wxT("Goto"), wxBitmap( wxT("./res/goto.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Goto an address in flash or RAM"), wxEmptyString );
	m_toolBar1->Realize();
	
	m_disasmGotoPanel->Hide();
	m_disasmView->GotoAddress(addr_to_waddr(&lpCalc->mem_c, lpCalc->cpu.pc));
	regPane->DebugUpdateWindow();
}


void CPU_stepout(LPCALC lpCalc) {
	CPU_t *cpu = &lpCalc->cpu;
	double time = tc_tstates(cpu->timer_c);
	uint16_t old_sp = cpu->sp;

	uint64_t tstates15seconds = 15.0 * cpu->timer_c->freq;
	while ((tc_tstates(cpu->timer_c) - time) < tstates15seconds) {
		waddr_t old_pc = addr_to_waddr(cpu->mem_c, cpu->pc);
		CPU_step(cpu);

		if (cpu->sp > old_sp) {
			Z80_info_t zinflocal;
			disassemble(lpCalc, REGULAR, addr_to_waddr(cpu->mem_c, old_pc.addr), 1, &zinflocal);

			if (zinflocal.index == DA_RET 		||
				zinflocal.index == DA_RET_CC 	||
				zinflocal.index == DA_RETI		||
				zinflocal.index == DA_RETN) {
				
				return;
			}

		}
	}
}

/*
 * Repeatedly step until you get to the next command
 */
void CPU_stepover(LPCALC lpCalc) {
	const int usable_commands[] = { DA_BJUMP, DA_BJUMP_N, DA_BCALL_N, DA_BCALL,
									DA_BLI, DA_CALL_X, DA_CALL_CC_X, DA_HALT, DA_RST_X};
	int i;
	CPU_t *cpu = &lpCalc->cpu;
	double time = tc_elapsed(cpu->timer_c);
	Z80_info_t zinflocal;

	disassemble(lpCalc, REGULAR, addr_to_waddr(cpu->mem_c, cpu->pc), 1, &zinflocal);

	if (cpu->halt) {
		if (cpu->iff1) {
			while ((tc_elapsed(cpu->timer_c) - time) < 15.0 && cpu->halt == TRUE )
				CPU_step(cpu);
		} else {
			cpu->halt = FALSE;
		}
	} else if (zinflocal.index == DA_CALL_X || zinflocal.index == DA_CALL_CC_X) {
		uint16_t old_stack = cpu->sp;
		uint16_t return_pc = cpu->pc + 3;
		CPU_step(cpu);
		if (cpu->sp != old_stack) {
			double time = tc_elapsed(cpu->timer_c);
			uint16_t old_sp = cpu->sp;
			while ((tc_elapsed(cpu->timer_c) - time) < 15.0) {
				uint16_t old_pc = cpu->pc;
				CPU_step(cpu);

				if (cpu->sp >= old_sp && (cpu->pc >= return_pc && (cpu->pc <= return_pc + 2))) {
					Z80_info_t zinflocal;
					disassemble(lpCalc, REGULAR, addr_to_waddr(cpu->mem_c, old_pc), 1, &zinflocal);

					if (zinflocal.index == DA_RET 		||
						zinflocal.index == DA_RET_CC 	||
						zinflocal.index == DA_RETI		||
						zinflocal.index == DA_RETN) {
				
						return;
					}

				}
			}
		}
	} else {
		for (i = 0; i < ARRAYSIZE(usable_commands); i++) {
			if (zinflocal.index == usable_commands[i]) {
				while ((tc_elapsed(cpu->timer_c) - time) < 15.0 && cpu->pc != (zinflocal.waddr.addr + zinflocal.size))
					CPU_step(cpu);
				return;
			}
		}

		CPU_step(cpu);
	}
}

void WabbitemuDebugger::OnClose(wxCloseEvent& event) {
	calc_unpause_linked();
	lpCalc->running =TRUE;
	event.Skip();
}

void WabbitemuDebugger::DebugUpdateWindow() {
	regPane->DebugUpdateWindow();
}

void WabbitemuDebugger::OnToolbarRun(wxCommandEvent & WXUNUSED(event)) {
	if (lpCalc->running) {
		lpCalc->running = FALSE;
		calc_pause_linked();
		this->m_disasmView->Enable();
		m_toolBar1->DeleteTool(ID_Toolbar_Run);
		m_toolBar1->InsertTool(0, ID_Toolbar_Run, wxT("Run"), wxBitmap( wxT("./res/run.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Run the calculator"), wxEmptyString );
		m_disasmView->GotoAddress(addr_to_waddr(&lpCalc->mem_c, lpCalc->cpu.pc));
	} else {
		lpCalc->running = TRUE;
		calc_unpause_linked();
		m_disasmView->Disable();
		m_toolBar1->DeleteTool(ID_Toolbar_Run);
		m_toolBar1->InsertTool(0, ID_Toolbar_Run, wxT("Stop"), wxBitmap( wxT("./res/stop.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Run the calculator"), wxEmptyString );
	}
}

void WabbitemuDebugger::OnToolbarBreak(wxCommandEvent & WXUNUSED(event)) {
	int itemIndex = m_disasmView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (itemIndex == -1) {
		return;
	}
	if (check_break(&lpCalc->mem_c, m_disasmView->MapIndexToAddress(itemIndex))) {
		clear_break(&lpCalc->mem_c, m_disasmView->MapIndexToAddress(itemIndex));
	} else {
		set_break(&lpCalc->mem_c, m_disasmView->MapIndexToAddress(itemIndex));
	}
	DebugUpdateWindow();
}

void WabbitemuDebugger::OnToolbarMemBreak(wxCommandEvent & WXUNUSED(event)) {
	int itemIndex = m_disasmView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (itemIndex == -1) {
		return;
	}
	if (check_mem_write_break(&lpCalc->mem_c, m_disasmView->MapIndexToAddress(itemIndex))) {
		clear_mem_write_break(&lpCalc->mem_c, m_disasmView->MapIndexToAddress(itemIndex));
	} else {
		set_mem_write_break(&lpCalc->mem_c, m_disasmView->MapIndexToAddress(itemIndex));
	}
	DebugUpdateWindow();
}

void WabbitemuDebugger::OnToolbarStep(wxCommandEvent & WXUNUSED(event)) {
	CPU_step(&lpCalc->cpu);
	m_disasmView->Refresh();
	m_disasmView->Update();
	m_disasmView->GotoAddress(addr_to_waddr(&lpCalc->mem_c, lpCalc->cpu.pc));
	DebugUpdateWindow();
}

void WabbitemuDebugger::OnToolbarGoto(wxCommandEvent & WXUNUSED(event)) {
	bSizer3->Layout();
	m_disasmGotoPanel->Show();
}

void WabbitemuDebugger::OnDisasmGoto(wxCommandEvent & WXUNUSED(event)) {
	std::stringstream ss;
	int addressToGoto;
	ss << std::hex << m_disasmGotoText->GetValue(); 
	ss >> addressToGoto;
	waddr_t waddr;
	switch (m_disasmView->viewType) {
		case REGULAR:
			waddr = addr_to_waddr(&lpCalc->mem_c, addressToGoto);
			break;
	}
	m_disasmView->GotoAddress(waddr);
	m_disasmGotoPanel->Hide();
}

void WabbitemuDebugger::OnDisasmGotoEnter(wxCommandEvent & event) {
	OnDisasmGoto(event);
}

void WabbitemuDebugger::OnToolbarStepOver(wxCommandEvent & WXUNUSED(event)) {
	CPU_stepover(lpCalc);
	m_disasmView->Refresh();
	m_disasmView->Update();
	m_disasmView->GotoAddress(addr_to_waddr(&lpCalc->mem_c, lpCalc->cpu.pc));
	DebugUpdateWindow();
}