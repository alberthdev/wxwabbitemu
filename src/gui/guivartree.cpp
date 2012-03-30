#include "guivartree.h"
#include "fileutilities.h"
#include "state.h"
#include "tiicons.h"
#include "exportvar.h"

enum
{
	ID_Var_Refresh,
	ID_Var_Export
};

BEGIN_EVENT_TABLE(VarTree, wxFrame)
	EVT_TREE_SEL_CHANGED(wxID_ANY, VarTree::OnTreeSelChanged)
	EVT_BUTTON(ID_Var_Refresh, VarTree::OnRefresh)
	EVT_BUTTON(ID_Var_Export, VarTree::OnExport)
END_EVENT_TABLE()

extern char type_ext[][4];
VarTree::VarTree( wxWindow* parent) : wxFrame(parent, wxID_ANY, _T("Calculator Variables"),
	wxDefaultPosition, wxSize(350, 400), wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN )
{
	*export_file_name = '\0';
	Tree_init = false;
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	bSizer54 = new wxBoxSizer( wxVERTICAL );
	bSizer53 = new wxBoxSizer( wxHORIZONTAL );
	
	m_refreshButton = new wxButton( this, ID_Var_Refresh, wxT("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer53->Add( m_refreshButton, 0, wxALL, 5 );
	
	m_exportButton = new wxButton( this, ID_Var_Export, wxT("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer53->Add( m_exportButton, 0, wxALL, 5 );
	
	bSizer54->Add( bSizer53, 0, wxEXPAND, 5 );
	
	m_treeVariables = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer54->Add( m_treeVariables, 1, wxALL|wxEXPAND, 5 );
	
	m_staticNameText = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticNameText->Wrap( -1 );
	bSizer54->Add( m_staticNameText, 0, wxALL, 5 );
	
	gSizer9 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticAddressText = new wxStaticText( this, wxID_ANY, wxT("Address:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticAddressText->Wrap( -1 );
	gSizer9->Add( m_staticAddressText, 0, wxALL, 5 );
	
	m_staticRamText = new wxStaticText( this, wxID_ANY, wxT("In RAM:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticRamText->Wrap( -1 );
	gSizer9->Add( m_staticRamText, 0, wxALL, 5 );
	
	m_staticPageText = new wxStaticText( this, wxID_ANY, wxT("Page:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticPageText->Wrap( -1 );
	gSizer9->Add( m_staticPageText, 0, wxALL, 5 );
	
	bSizer54->Add( gSizer9, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer54 );
	this->Layout();
	
	imageList = new wxImageList(16, 16, true, 20);
	imageList->Add(wxImage(_T("./res/ti-icons.png")), wxColor(0, 0xFF, 0));
	m_treeVariables->AssignImageList(imageList);
	
	UpdateVarTree(true);
}

VarTree::~VarTree()
{
	delete m_staticPageText;
	m_staticPageText = NULL;
	delete m_staticRamText;
	m_staticRamText = NULL;
	delete m_staticAddressText;
	m_staticAddressText = NULL;
	delete m_staticNameText;
	m_staticNameText = NULL;
}

void VarTree::OnTreeSelChanged(wxTreeEvent &event)
{
	char string[MAX_PATH];
	apphdr_t *app;
	symbol83P_t *symbol;
	int slot;
	wxTreeItemId item = event.GetItem();
	app = GetAppVariable(item, slot);
	if (!app) {
		symbol = GetSymbolVariable(item, slot);
	}
	if (app) {
		if (App_Name_to_String(app, string)) {
			m_staticNameText->SetLabel(string);
			sprintf(string, "Page: %02X", app->page);
			m_staticPageText->SetLabel(string);
		}
	} else if (symbol) {
		if (Symbol_Name_to_String(Tree[slot].model, symbol, string)) {
			m_staticNameText->SetLabel(string);
			sprintf(string, "Address: %04X", symbol->address);
			m_staticAddressText->SetLabel(string);
			sprintf(string, "Page: %02X", symbol->page);
			m_staticPageText->SetLabel(string);
			m_staticRamText->SetLabel(symbol->page == 0 || Tree[slot].model == TI_86 ?
											"In RAM: True" : "In RAM: False");
		}
	}
}

void VarTree::OnRefresh(wxCommandEvent &event)
{
	UpdateVarTree(false);
}

void VarTree::OnExport(wxCommandEvent &event)
{
	wxTreeItemId item = m_treeVariables->GetSelection();
	FILE *file;
	char *buf;
	char filePath[MAX_PATH];
	int size = FillDesc(item, filePath);
	if (size == -1) {
		return;
	}
	buf =  (char *) malloc(size);
	FillFileBuffer(item, buf);
	if (SetVarName(filePath) == -1) {
		return;
	}
	file = fopen(export_file_name, "wb");
	fwrite(buf, 1, size, file);
	fclose(file);
	free(buf);
}

int VarTree::FillDesc(wxTreeItemId &hSelect, char *filePath) {
	int slot;
	u_int i;
	char string[MAX_PATH];
	memset(string, 0, sizeof(string));
	for(slot = 0; slot < MAX_CALCS; slot++) {
		if (Tree[slot].model) {
			for(i = 0; i < Tree[slot].applist.count; i++) {
				if (Tree[slot].hApps[i] && *Tree[slot].hApps[i] == hSelect) {
					if (App_Name_to_String(&Tree[slot].applist.apps[i], string)) {
						strcat(string, ".8xk");
						strcpy(filePath, string);
						MFILE *outfile = ExportApp(&calcs[slot], NULL, &Tree[slot].applist.apps[i]);
						int size = msize(outfile);
						mclose(outfile);
						return size;
					}
					return -1;
				}
			}
			if (Tree[slot].sym.last == NULL) {
				continue;
			}
			for(i = 0; i < (u_int) (Tree[slot].sym.last - Tree[slot].sym.symbols + 1); i++) {
				if (Tree[slot].hVars[i] && *Tree[slot].hVars[i] == hSelect) {
					if (Symbol_Name_to_String(Tree[slot].model, &Tree[slot].sym.symbols[i], string)) {
						strcat(string, ".");
						strcat(string, (const char *) type_ext[Tree[slot].sym.symbols[i].type_ID]);
						strcpy(filePath, string);
						MFILE *outfile = ExportVar(&calcs[slot], NULL, &Tree[slot].sym.symbols[i]);
						int size = msize(outfile);
						mclose(outfile);
						return size;
					}
				}
			}
		}
	}
	return -1;
}

void *VarTree::FillFileBuffer(wxTreeItemId &hSelect, void *buf) {
	u_int slot, i, b;
	unsigned char *buffer = (unsigned char *) buf;
	char string[64];
	memset(string, 0, sizeof(string));
	for(slot = 0; slot < MAX_CALCS; slot++) {
		if (Tree[slot].model) {
			for(i = 0; i < Tree[slot].applist.count; i++) {
				if (Tree[slot].hApps[i] && *Tree[slot].hApps[i] == hSelect) {
					MFILE *outfile = ExportApp(&calcs[slot], NULL, &Tree[slot].applist.apps[i]);
					if(!outfile) {
						return NULL;
					}
					for(b = 0; b < outfile->size; b++) {
						buffer[b] = outfile->data[b];
					}
					mclose(outfile);
					return buffer;
				}
			}
			for(i = 0; i < (u_int) (Tree[slot].sym.last - Tree[slot].sym.symbols + 1); i++) {
				if (Tree[slot].hVars[i] && *Tree[slot].hVars[i] == hSelect) {
					if (Symbol_Name_to_String(Tree[slot].model, &Tree[slot].sym.symbols[i], string)) {
						MFILE *outfile = ExportVar(&calcs[slot], NULL, &Tree[slot].sym.symbols[i]);
						if(!outfile) {
							return NULL;
						}
						for(b = 0; b < outfile->size; b++) {
							buffer[b] = outfile->data[b];
						}
						mclose(outfile);
						return buffer;
					}
				}
			}
		}
	}
	return NULL;
}

//TODO: well this code is a mess. We need to refactor this, so that the HTREEITEM is somehow
//mapped to the to either the symlist_t or applist_t item. Ideally this would be the LPARAM 
//of the LPTREEVIEW, but a dictionary mapping would be fine as well
apphdr_t *VarTree::GetAppVariable(wxTreeItemId &hTreeItem, int &slot) {
	for (int temp = 0; temp < MAX_CALCS; temp++) {
		if (Tree[temp].model) {
			for(u_int i = 0; i < Tree[temp].applist.count; i++) {
				if (*Tree[temp].hApps[i] == hTreeItem) {
					slot = temp;
					return &Tree[temp].applist.apps[i];
				}
			}
		}
	}
	return NULL;
}

symbol83P_t *VarTree::GetSymbolVariable(wxTreeItemId &hTreeItem, int &slot) {
	for (int temp = 0; temp < MAX_CALCS; temp++) {
		if (Tree[temp].model) {
			if (Tree[temp].sym.last == NULL || Tree[temp].sym.symbols == NULL) {
				continue;
			}
			int numSymbols = (Tree[temp].sym.last - Tree[temp].sym.symbols + 1);
			if (numSymbols < 0) {
				continue;
			}
			for(u_int i = 0; i < numSymbols; i++) {
				if (Tree[temp].hVars[i] && *Tree[temp].hVars[i] == hTreeItem) {
					slot = temp;
					return &Tree[temp].sym.symbols[i];
				}
			}
		}
	}
	return NULL;
}

int VarTree::SetVarName(char *filePath) {
	char *defExt;
	int filterIndex;
	const char lpstrFilter[] = "Programs  (*.8xp)|*.8xp|Applications (*.8xk)|*.8xk|App Vars (*.8xv)|*.8xv|Lists  (*.8xl)|*.8xl|Real/Complex Variables  (*.8xn)|*.8xn|\
Pictures  (*.8xi)|*.8xi|GDBs  (*.8xd)|*.8xd|Matrices  (*.8xm)|*.8xm|Strings  (*.8xs)|*.8xs|Groups  (*.8xg)|*.8xg|All Files (*.*)|*.*";
	const char lpstrTitle[] = "Wabbitemu Export";
	char lpstrFile[MAX_PATH];
	strcpy(lpstrFile, filePath);
	size_t i = _tcslen(lpstrFile);
	lpstrFile[i] = '\0';
	defExt = &lpstrFile[i];
	while (*defExt != '.')
		defExt--;
	switch (defExt[3]) {
		case 'p':
			filterIndex = 0;
			break;
		case 'k':
			filterIndex = 1;
			break;
		case 'v':
			filterIndex = 2;
			break;
		case 'l':
			filterIndex = 3;
			break;
		case 'n':
			filterIndex = 4;
			break;
		case 'i':
			filterIndex = 5;
			break;
		case 'd':
			filterIndex = 6;
			break;
		case 'm':
			filterIndex = 7;
			break;
		case 's':
			filterIndex = 8;
			break;
		case 'g':
			filterIndex = 9;
			break;
		default:
			filterIndex = 10;
			break;
	}

	if (SaveFile(lpstrFile, lpstrFilter, lpstrTitle, defExt, wxFD_FILE_MUST_EXIST , filterIndex)) {
		return -1;
	}
	strcpy(export_file_name, lpstrFile);
	return 0;
}

void VarTree::UpdateVarTree(bool New)
{
	unsigned int i;
	int slot;
	float ver;

	if (Tree_init == false || New != false) {
		memset(Tree, 0, sizeof(Tree));
		Tree_init = true;
	}

	/* run through all active calcs */
	for(slot = 0; slot < MAX_CALCS; slot++) {
		if (!calcs[slot].active && Tree[slot].model != 0) {
			m_treeVariables->Delete(Tree[slot].hRoot);
			Tree[slot].model = 0;
		}

		if (calcs[slot].active && Tree[slot].model != calcs[slot].model && Tree[slot].model != 0) {
			m_treeVariables->Delete(Tree[slot].hRoot);
			Tree[slot].model = 0;
		}
		
		/*It's an 83+ compatible with a known rom(hopefully)*/
		if (calcs[slot].active && (calcs[slot].model >= TI_83P || calcs[slot].model == TI_86) &&
			sscanf(calcs[slot].rom_version, "%f", &ver) == 1) {

			/* This slot has not yet been initialized. */
			/* so set up the Root */
			if (Tree[slot].model == 0) {
				Tree[slot].hRoot = m_treeVariables->AddRoot(CalcModelTxt[calcs[slot].model], TI_ICON_84PSE);
			}

			/* If nodes haven't been init or the model is reset, create nodes */
			/* otherwise delete children so the vars can be appended */
			if (!Tree[slot].hApplication || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Application"), TI_ICON_APP);
				Tree[slot].hApplication = new wxTreeItemId(item);
				
			} else {
				m_treeVariables->DeleteChildren(Tree[slot].hApplication);
			}

			if (!Tree[slot].hProgram || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Program"), TI_ICON_PROGRAM);
				Tree[slot].hProgram = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hProgram);
			}

			if (!Tree[slot].hAppVar || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Application Variable"));
				Tree[slot].hAppVar = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hAppVar);
			} 

			if (!Tree[slot].hPic || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Picture"), TI_ICON_PIC);
				Tree[slot].hPic = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hPic);
			}

			if (!Tree[slot].hGDB || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Graph Database"), TI_ICON_GDB);
				Tree[slot].hGDB = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hGDB);
			}

			if (!Tree[slot].hString || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("String"), TI_ICON_STRING);
				Tree[slot].hString = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hString);
			}

			if (!Tree[slot].hNumber || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Number"), TI_ICON_NUMBER);
				Tree[slot].hNumber = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hNumber);
			}

			if (!Tree[slot].hList || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("List"), TI_ICON_LIST);
				Tree[slot].hList = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hList);
			}

			if (!Tree[slot].hMatrix || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Matrix"), TI_ICON_MATRIX);
				Tree[slot].hMatrix = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hMatrix);
			}

			if (!Tree[slot].hGroup || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Group"), TI_ICON_GROUP);
				Tree[slot].hGroup = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hGroup);
			}

			if (!Tree[slot].hEquation || Tree[slot].model == 0) {
				wxTreeItemId item = m_treeVariables->AppendItem(Tree[slot].hRoot, _T("Equation"), TI_ICON_EQUATIONS);
				Tree[slot].hEquation = new wxTreeItemId(item);
			} else {
				m_treeVariables->DeleteChildren(*Tree[slot].hEquation);
			}

			Tree[slot].model		= calcs[slot].cpu.pio.model;
			
			/* Apps are handled outside of the symbol table*/
			state_build_applist(&calcs[slot].cpu, &Tree[slot].applist);
			for(i = 0; i < Tree[slot].applist.count; i++) {
				wxTreeItemId item = m_treeVariables->AppendItem(*Tree[slot].hApplication, Tree[slot].applist.apps[i].name, TI_ICON_FILE_ARC);
				Tree[slot].hApps[i] = new wxTreeItemId(item);
			}
			symlist_t* sym;
			if (calcs[slot].model == TI_86) {
				sym = state_build_symlist_86(&calcs[slot].cpu, &Tree[slot].sym);
			} else {
				sym = state_build_symlist_83P(&calcs[slot].cpu, &Tree[slot].sym);
			}
			if (sym) {
				// FIXME
				for(i = 0; (&sym->symbols[i]) <= sym->last; i++) {
					TCHAR tmpstring[64];
					int icon;
					
					/* whether its archived or not */
					/* depends on the page its stored */
					if (sym->symbols[i].page && Tree[slot].model != TI_86) {
						icon = TI_ICON_FILE_ARC;  //red
					} else {
						icon = TI_ICON_FILE_RAM;  //green
					}
					
					if (Symbol_Name_to_String(Tree[slot].model, &sym->symbols[i], tmpstring)) {
						wxTreeItemId item;
						if (Tree[slot].model == TI_86) {
							switch(sym->symbols[i].type_ID) {
								case ProgObj86:
									item = m_treeVariables->AppendItem(*Tree[slot].hProgram, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case PictObj86:
									item = m_treeVariables->AppendItem(*Tree[slot].hPic, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case FuncGDBObj86:
								case DiffEquGDBObj86:
								case ParamGDBObj86:
								case PolarGDBObj86:
									item = m_treeVariables->AppendItem(*Tree[slot].hGDB, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case StrngObj86:
									item = m_treeVariables->AppendItem(*Tree[slot].hString, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case ConstObj86:
								case RealObj86:
								case CplxObj86:
									item = m_treeVariables->AppendItem(*Tree[slot].hNumber, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case ListObj86:
								case CListObj86:
									item = m_treeVariables->AppendItem(*Tree[slot].hList, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case MatObj86:
								case CMatObj86:
								case VectObj86:
									item = m_treeVariables->AppendItem(*Tree[slot].hMatrix, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case EquObj86:
								case EquObj_286:
									item = m_treeVariables->AppendItem(*Tree[slot].hEquation, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
							}
						} else {
							switch(sym->symbols[i].type_ID) {
								case ProgObj:
								case ProtProgObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hProgram, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case AppVarObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hAppVar, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case GroupObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hGroup, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case PictObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hPic, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case GDBObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hGDB, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case StrngObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hString, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case RealObj:
								case CplxObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hNumber, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case ListObj:
								case CListObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hList, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case MatObj:
									item = m_treeVariables->AppendItem(*Tree[slot].hMatrix, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
								case EquObj_2:
									item = m_treeVariables->AppendItem(*Tree[slot].hEquation, tmpstring, icon);
									Tree[slot].hVars[i] = new wxTreeItemId(item);
									break;
							}
						}
					}
				}
				//free(sym);
			}
			// If no children are found kill parent.
			for(i = 0; i < 11; i++) {
				if (Tree[slot].hTypes[i]) {
					if (m_treeVariables->GetChildrenCount(*Tree[slot].hTypes[i]) == 0) {
						m_treeVariables->Delete(*Tree[slot].hTypes[i]);
						Tree[slot].hTypes[i] = NULL;
					}
				}
			}
			m_treeVariables->SortChildren(Tree[slot].hRoot);
			m_treeVariables->Expand(Tree[slot].hRoot);
		}
	}
}