#include "stdafx.h"
#include "disassemblyview.h"
#include "calc.h"

extern Z80_com_t da_opcode[256];

int DisassemblyView::FindLastItem() {
	bool doneBank0 = false;
	int i;
	for (i = 0; i < 0xFFFF; i++) {
		if (!doneBank0 && (zinf[i].waddr.addr >> 14) == 1) {
			doneBank0 = true;
		}
		if (doneBank0 && (zinf[i].waddr.addr >> 14) == 0) {
			break;
		}
	}
	return i;
}

DisassemblyView::DisassemblyView(wxWindow *parent, LPCALC lpCalc, ViewType type):
					wxListCtrl(parent,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_VIRTUAL){
	this->lpCalc = lpCalc;
	this->viewType = type;
	wxListItem col0; 
	col0.SetId(0); 
	col0.SetText( _T("Address") ); 
	col0.SetWidth(70);
	InsertColumn(0, col0); 
 
	wxListItem col1; 
	col1.SetId(1); 
	col1.SetText( _T("Data") );
	col1.SetWidth(77);
	InsertColumn(1, col1); 
		  
	wxListItem col2; 
	col2.SetId(2); 
	col2.SetText( _T("Disassembly") ); 
	col2.SetWidth(315);
	InsertColumn(2, col2); 
	
	wxListItem col3; 
	col3.SetId(3); 
	col3.SetText( _T("Size") ); 
	col3.SetWidth(42);
	InsertColumn(3, col3); 
	
	wxListItem col4; 
	col4.SetId(4); 
	col4.SetText( _T("Clocks") ); 
	col4.SetWidth(56);
	InsertColumn(4, col4); 
	
	zinf = new Z80_info_t[0xFFFF];
	memset(zinf, 0, sizeof(Z80_info_t) * 0xFFFF);
	waddr_t waddr = addr_to_waddr(&lpCalc->mem_c, 0);
	disassemble(lpCalc, REGULAR, waddr, 0xFFFF, zinf);
	
	SetItemCount(FindLastItem());
}
 
wxListItemAttr *DisassemblyView::OnGetItemAttr(long item) const {
	if (lpCalc->cpu.pc == zinf[item].waddr.addr) {
		wxColor pcColor;
		if (lpCalc->cpu.halt) {
			pcColor = wxColor(200, 200, 100);
		} else {
			pcColor = wxColor(180, 180, 180);
		}
		
		return new wxListItemAttr(*wxBLACK, pcColor, *wxNORMAL_FONT);
	}
	if (check_break(&lpCalc->mem_c, zinf[item].waddr)) {
		return new wxListItemAttr(*wxWHITE, wxColor(230, 160, 180), *wxNORMAL_FONT);
	}
	if (check_mem_write_break(&lpCalc->mem_c, zinf[item].waddr)) {
		return new wxListItemAttr(*wxWHITE, wxColor(255, 177, 100), *wxNORMAL_FONT);
	}
	if (check_mem_read_break(&lpCalc->mem_c, zinf[item].waddr)) {
		return new wxListItemAttr(*wxWHITE, wxColor(255, 250, 145), *wxNORMAL_FONT);
	}
	return wxListCtrl::OnGetItemAttr(item);
}

int DisassemblyView::MapAddressToIndex(waddr_t &waddr) {
	for (int i = 0; i < 0xFFFF; i++) {
		if (zinf[i].waddr.addr == waddr.addr && zinf[i].waddr.page == waddr.page && zinf[i].waddr.is_ram == waddr.is_ram) {
			return i;
		}
		if (zinf[i].waddr.addr > waddr.addr && zinf[i].waddr.page == waddr.page && zinf[i].waddr.is_ram == waddr.is_ram) {
			return i - 1;
		}
	}
	return -1;
}

waddr_t& DisassemblyView::MapIndexToAddress(int index) {
	return zinf[index].waddr;
}

void DisassemblyView::DebugUpdateWindow() {
	this->Refresh();
}

void DisassemblyView::GotoAddress(waddr_t waddr) {
	int i = MapAddressToIndex(waddr);
	EnsureVisible(i);
}
 
wxString DisassemblyView::OnGetItemText(long item, long column) const{
	TCHAR s[64];
	memset(s, 0, sizeof(s));
	switch (column) {
		case 0:
			sprint_addr(lpCalc, &zinf[item], s);
			break;
		case 1:
			sprint_data(lpCalc, &zinf[item], s);
			break;
		case 2:
			sprint_command(lpCalc, &zinf[item], s);
			break;
		case 3:
			sprint_size(lpCalc, &zinf[item], s);
			break;
		case 4:
			sprint_clocks(lpCalc, &zinf[item], s);
			break;
	}
	return wxString(s);
}

void DisassemblyView::sprint_addr(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const {
	int page = zinf->waddr.page;
	if (zinf->waddr.is_ram) {
		switch (lpCalc->cpu.pio.model) {
			case TI_83P:
			case TI_73:
				page += 0x40;
				break;
			case TI_83PSE:
			case TI_84P:
			case TI_84PSE:
				page += 0x80;
				break;
		}
	}
	_tprintf(s, _T("%02X %04X"), page, zinf->waddr.addr);
}

void DisassemblyView::sprint_data(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const {
	int j;

	if (zinf->size == 0) {
		return;	
	}

	waddr_t waddr = zinf->waddr;
	for (j = 0; j < zinf->size; j++) {
		waddr.addr = zinf->waddr.addr + j;
		if (waddr.addr % PAGE_SIZE < zinf->waddr.addr % PAGE_SIZE) {
			waddr.page++;
			//we don't handle ram changes here because things should never cross pages
			//therefore i don't really care
		}
		_tprintf(s + (j*2), _T("%02x"), wmem_read(lpCalc->cpu.mem_c, waddr));
	}
}

void DisassemblyView::sprint_command(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const {
	_tcscpy(s, zinf->expanded);
}

void DisassemblyView::sprint_size(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const {
	if (zinf->size == 0) {
		return;
	}
	_tprintf(s, _T("%d"), zinf->size);
}

void DisassemblyView::sprint_clocks(LPCALC lpCalc, const Z80_info_t *zinf, TCHAR *s) const {
	if (da_opcode[zinf->index].clocks != -1) {
		if (da_opcode[zinf->index].clocks_cond) {
			_tprintf(s, _T("%d/%d"), da_opcode[zinf->index].clocks, da_opcode[zinf->index].clocks_cond);
		} else {
			_tprintf(s, _T("%d"), da_opcode[zinf->index].clocks);
		}
	} else {
		*s = '\0';
	}
}