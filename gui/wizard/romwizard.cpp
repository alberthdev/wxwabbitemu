#include "romwizard.h"
#include "gui.h"
#include "fileutilities.h"
#include "exportvar.h"
#include "linksendvar.h"
#include "var.h"

#include "rom/bf73.h"
#include "rom/bf83p.h"
#include "rom/bf83pse.h"
#include "rom/bf84p.h"
#include "rom/bf84pse.h"

BEGIN_EVENT_TABLE(RomWizard, wxWizard)
	EVT_WIZARD_FINISHED(wxID_ANY, RomWizard::OnFinish)
	EVT_WIZARD_PAGE_CHANGED(wxID_ANY, RomWizard::OnPageChanged)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY, RomWizard::OnPageChanging)
END_EVENT_TABLE()

RomWizard::RomWizard() : wxWizard (NULL, wxID_ANY, _T("ROM Wizard")) {
	startPage = new WizardStartPage(this);
	calcTypePage = new WizardCalcTypePage(this);
	startPage->next = calcTypePage;
	calcTypePage->prev = startPage;
	osPage = new WizardOSPage(this);
	calcTypePage->next = osPage;
	osPage->prev = calcTypePage;
	SetPageSize(startPage->GetBestSize());
	
	// Set defaults for the start page
	wxWindow *win = FindWindowById(wxID_FORWARD, this);
	win->SetLabel(wxT("Finish"));
	win->Enable(false);
}

bool RomWizard::Begin() {
	return this->RunWizard(startPage);
}

// Dirty hack to prevent buggy wx versions from moving forward
// or backwards when the button itself is disabled
void RomWizard::OnPageChanging(wxWizardEvent &event) {
	wxWindow *win;
	if (event.GetDirection()) {
		// Going forward
		win = FindWindowById(wxID_FORWARD, this);
	} else {
		// Going backward
		win = FindWindowById(wxID_BACKWARD, this);
	}
	
	// If the button isn't enabled, veto the event!
	if (!win->IsEnabled()) {
		event.Veto();
	}
}

void RomWizard::OnPageChanged(wxWizardEvent &event) {
	if (calcTypePage == event.GetPage()) {
		wxWindow *win = FindWindowById(wxID_FORWARD, this);
		if (startPage->m_copyRadio->GetValue()) {
			calcTypePage->EnableRadios(true);
		} else {
			calcTypePage->EnableRadios(false);
		}
		win->Enable(true);
	} else if (osPage == event.GetPage()) {
		osPage->creatingROM = startPage->m_createRadio->GetValue();
		wxWindow *win = FindWindowById(wxID_FORWARD, this);
		if (osPage->creatingROM) {
			win->SetLabel(_T("Finish"));
		} else {
			win->SetLabel(_T("Next >"));
		}
		osPage->UpdateOSPathState();
	}
}

void RomWizard::ModelInit(LPCALC lpCalc, int model)
{
	calc_init_model(lpCalc, model, NULL);
}

BOOL RomWizard::ExtractBootFree(wxString &bootfreePath, int model) {
	wxString tempFile = wxFileName::GetTempDir();
	tempFile.Append(_T("/bootfree.hex"));
	bootfreePath = tempFile;
	FILE *file = fopen(tempFile.mb_str(), "wb");
	const unsigned char *output;
	size_t size;
	switch(model) {
		case TI_73:
			output = bf73_hex;
			size = sizeof(bf73_hex);
			break;
		case TI_83P:
			output = bf83pbe_hex;
			size = sizeof(bf83pbe_hex);
			break;
		case TI_83PSE:
			output = bf83pse_hex;
			size = sizeof(bf83pse_hex);
			break;
		case TI_84P:
			output = bf84pbe_hex;
			size = sizeof(bf84pbe_hex);
			break;
		case TI_84PSE:
			output = bf84pse_hex;
			size = sizeof(bf84pse_hex);
			break;
	}
	for (int i = 0; i < size; i++) {
		fputc(output[i], file);
	}
	fclose(file);
	return TRUE;
}

void RomWizard::OnFinish(wxWizardEvent &event) {
	if (startPage->m_browseRadio->GetValue()) {
		wxString path = startPage->m_filePicker1->GetPath();
		LPCALC lpCalc = calc_slot_new();
		BOOL success = rom_load(lpCalc, path.c_str());
		if (!success) {
			//should never get here
			return;
		}
		gui_frame(lpCalc);
		return;
	}
	if (startPage->m_createRadio->GetValue()) {
		TCHAR buffer[255];
		wxString osPath;
		int model = calcTypePage->GetModel();
		
		if (!SaveFile(buffer, _T("ROMs  (*.rom)\0*.rom\0Bins  (*.bin)\0*.bin\0All Files (*.*)\0*.*\0\0"),
					_T("Wabbitemu Export Rom"), _T("rom"))) {
			printf("Going for save! [%254s]\n", buffer);
			osPath = osPage->m_filePicker2->GetPath();
			
			//if you don't want to load an OS, fine...
			if (osPath.length() > 0) {
				TIFILE_t *tifile = importvar(osPath, FALSE);
				if (tifile == NULL || tifile->type != TifileVarType_t::FLASH_TYPE) {
					wxMessageBox(_T("Error: OS file is corrupt!"), _T("Error"), wxOK | wxICON_ERROR);
					return;
				} else {
					LPCALC lpCalc = calc_slot_new();
					//ok yes i know this is retarded...but this way we can use Load_8xu
					//outside this function...
					wxString hexFile;
					ExtractBootFree(hexFile, model);
					ModelInit(lpCalc, model);
					//slot stuff
					//LoadRegistrySettings(lpCalc);
					_tcscpy(lpCalc->rom_path, osPath.c_str());
					
					lpCalc->active = TRUE;
					lpCalc->model = static_cast<CalcModel>(model);
					lpCalc->cpu.pio.model = model;
					FILE *file = fopen(hexFile.fn_str(), "rb");
					writeboot(file, &lpCalc->mem_c, -1);
					fclose(file);
					remove(hexFile.fn_str());
					
					calc_erase_certificate(lpCalc->mem_c.flash,lpCalc->mem_c.flash_size);
					calc_reset(lpCalc);
					//calc_turn_on(lpCalc);
					gui_frame(lpCalc);
					//write the output from file
					MFILE *romfile = ExportRom(buffer, lpCalc);
					mclose(romfile);
					
					forceload_os(&lpCalc->cpu, tifile);
				}
			}
		}
	}
}
