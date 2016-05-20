#include "romwizard.h"
#include "gui.h"
#include "fileutilities.h"
#include "exportvar.h"

#include "rom/bf73.h"
#include "rom/bf83p.h"
#include "rom/bf83pse.h"
#include "rom/bf84p.h"
#include "rom/bf84pse.h"

BEGIN_EVENT_TABLE(RomWizard, wxWizard)
	EVT_WIZARD_FINISHED(wxID_ANY, RomWizard::OnFinish)
	EVT_WIZARD_PAGE_CHANGED(wxID_ANY, RomWizard::OnPageChanged)
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
}

bool RomWizard::Begin() {
	return this->RunWizard(startPage);
}

void RomWizard::OnPageChanged(wxWizardEvent &event) {
	if (calcTypePage == event.GetPage()) {
		if (startPage->m_copyRadio->GetValue()) {
			calcTypePage->EnableRadios(true);
		} else {
			calcTypePage->EnableRadios(false);
		}
	} else if (osPage == event.GetPage()) {
		osPage->creatingROM = startPage->m_createRadio->GetValue();
		osPage->m_choice1->Clear();
		switch (calcTypePage->GetModel()) {
			case TI_73:
				osPage->m_choice1->Append(_T("OS 1.91"));
				osPage->m_choice1->Enable(true);
				osPage->m_choice1->SetSelection(0);
				break;
			case TI_83P:
			case TI_83PSE:
				osPage->m_choice1->Append(_T("OS 1.19"));
				osPage->m_choice1->Enable(true);
				osPage->m_choice1->SetSelection(0);
				break;
			case TI_84P:
			case TI_84PSE:
				// No longer available!
				//osPage->m_choice1->Append(_T("OS 2.43"));
				osPage->m_choice1->Append(_T("OS 2.55 MP"));
				osPage->m_choice1->Enable(true);
				osPage->m_choice1->SetSelection(0);
				break;
			default:
				osPage->m_choice1->Enable(false);
				break;
		}
		wxWindow *win = FindWindowById(wxID_FORWARD, startPage->GetParent());
		if (osPage->creatingROM) {
			win->SetLabel(_T("Finish"));
		} else {
			win->SetLabel(_T("Next >"));
		}
	}
}

void RomWizard::ModelInit(LPCALC lpCalc, int model)
{
	switch(model) {
		case TI_73:
			calc_init_83p(lpCalc);
			break;
		case TI_83P:
			calc_init_83p(lpCalc);
			break;
		case TI_83PSE:
			calc_init_83pse(lpCalc);
			break;
		case TI_84P:
			calc_init_84p(lpCalc);
			break;
		case TI_84PSE:
			calc_init_83pse(lpCalc);
			break;
	}
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

BOOL RomWizard::DownloadOS(wxString &osFilePath, int model, BOOL version)
{
	wxString tempFile = wxFileName::GetTempDir();
	tempFile.Append(_T("/os.8xu"));
	osFilePath = tempFile;
	const TCHAR *url;
	switch (model) {
		case TI_73:
			url = _T("https://education.ti.com/download/en/US/956CE30854A74767893104FCDF195B76/32E99F6FAEB2424D8313B0DEE7B70791/TI73_OS.73u");
			break;
		case TI_83P:
		case TI_83PSE:
			url = _T("https://education.ti.com/download/en/US/377ABD4715984D858019EAFDC75FFD05/EEB252CDF6A748309894C1790408D0E7/TI83Plus_OS119.8Xu");
			break;
		case TI_84P:
		case TI_84PSE:
			url = _T("https://education.ti.com/download/en/US/B7DADA7FD4AA40CE9D7911B004B8C460/A943680938CC460E8CB04554E99D665B/TI84Plus_OS255.8Xu");
			break;
	}
	wxURL wxURL(url);
	if (wxURL.GetError() == wxURL_NOERR) {
		wxInputStream *input = wxURL.GetInputStream();
		// Ensure that we have an input stream in the first place!
		// If download fails, this will be NULL.
		if (input == NULL) {
			return FALSE;
		}
		
		FILE *file = fopen(tempFile.mb_str(), "wb");
		while (!input->Eof()){
			fputc(input->GetC(), file);
		}
		fclose(file);
		delete input;
		return TRUE;
	}
	return FALSE;
}

void RomWizard::OnFinish(wxWizardEvent &event) {
	if (startPage->m_browseRadio->GetValue()) {
		wxString path = startPage->m_filePicker1->GetPath();
		LPCALC lpCalc = calc_slot_new();
		BOOL success = rom_load(lpCalc, path.c_str());
		if (!success) {
			//should never get here
			event.Veto();
			return;
		}
		gui_frame(lpCalc);
		return;
	}
	if (startPage->m_createRadio->GetValue()) {
		TCHAR buffer[255];
		wxString osPath;
		SaveFile(buffer, _T("ROMs  (*.rom)\0*.rom\0Bins  (*.bin)\0*.bin\0All Files (*.*)\0*.*\0\0"),
					_T("Wabbitemu Export Rom"), _T("rom"));
		int model = calcTypePage->GetModel();
		if (osPage->m_downloadOS->GetValue()) {
			bool succeeded = DownloadOS(osPath, model, osPage->m_choice1->GetSelection() == 0);
			if (!succeeded) {
				wxMessageBox(_T("Error: Unable to download file!"), _T("Download failed"), wxOK | wxICON_ERROR);
				return;
			}
		} else {
			osPath = osPage->m_filePicker2->GetPath();
		}
		
		//if you don't want to load an OS, fine...
		if (osPath.length() > 0) {
			TIFILE_t *tifile = newimportvar(osPath);
			if (tifile == NULL || tifile->type != FLASH_TYPE) {
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
				lpCalc->model = model;
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
				if (osPage->m_downloadOS->GetValue()) {
					remove(osPath.fn_str());
				}
			}
		}
	}
}
