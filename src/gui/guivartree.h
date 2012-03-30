#ifndef GUIVARTREE_H
#define GUIVARTREE_H

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/imaglist.h>

#include "calc.h"
#include "link.h"

typedef struct{
	int model;
	wxTreeItemId hRoot;
	union {
		struct {
			wxTreeItemId *hAppVar;
			wxTreeItemId *hEquation;
			wxTreeItemId *hGDB;
			wxTreeItemId *hList;
			wxTreeItemId *hMatrix;
			wxTreeItemId *hNumber;
			wxTreeItemId *hPic;
			wxTreeItemId *hProgram;
			wxTreeItemId *hString;
			wxTreeItemId *hGroup;
			wxTreeItemId *hApplication;
		};
		wxTreeItemId *hTypes[11];
	};
	int count;
	wxTreeItemId *hApps[96];
	applist_t applist;
	wxTreeItemId *hVars[512];
	symlist_t sym;
} VARTREEVIEW_t;

enum { 
	TI_ICON_BLANK,
	TI_ICON_84PSE,
	TI_ICON_ARCHIVE,
	TI_ICON_APPVAR,
	TI_ICON_DEVICE_SETTINGS,
	TI_ICON_EQUATIONS,
	TI_ICON_GDB,
	TI_ICON_LIST,
	TI_ICON_MATRIX,
	TI_ICON_NUMBER,
	TI_ICON_PROGRAM,
	TI_ICON_STRING,
	TI_ICON_PIC,
	TI_ICON_APP,
	TI_ICON_GROUP,
	TI_ICON_UKNOWN,
	TI_ICON_FILE,
	TI_ICON_FILE_RAM,
	TI_ICON_FILE_ARC
};

class VarTree : public wxFrame {
private:
	wxButton* m_refreshButton;
	wxButton* m_exportButton;
	wxTreeCtrl* m_treeVariables;
	wxStaticText* m_staticAddressText;
	wxStaticText* m_staticRamText;
	wxStaticText* m_staticPageText;
	wxStaticText* m_staticNameText;
	wxImageList* imageList;
	wxBoxSizer* bSizer53;
	wxBoxSizer* bSizer54;
	wxGridSizer* gSizer9;
	
	apphdr_t *GetAppVariable(wxTreeItemId &hTreeItem, int &slot);
	symbol83P_t *GetSymbolVariable(wxTreeItemId &hTreeItem, int &slot);
	int FillDesc(wxTreeItemId &hSelect, char *filePath);
	void *FillFileBuffer(wxTreeItemId &hSelect, void *buf);
	int SetVarName(char *filePath);
	void OnTreeSelChanged(wxTreeEvent &event);
	void OnRefresh(wxCommandEvent &event);
	void OnExport(wxCommandEvent &event);
	
	VARTREEVIEW_t Tree[MAX_CALCS];
	char export_file_name[512];
	bool Tree_init;
protected:
	DECLARE_EVENT_TABLE()
public:
	VarTree(wxWindow *window);
	~VarTree();
	void UpdateVarTree(bool New);
};

#endif