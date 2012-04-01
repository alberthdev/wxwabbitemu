#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/dnd.h>
#include "calc.h"

class DnDFile : public wxFileDropTarget
{
public:
    DnDFile(wxWindow *pOwner, LPCALC lpCalc) { m_pOwner = pOwner; this->lpCalc = lpCalc; }

    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	LPCALC lpCalc;
    wxWindow *m_pOwner;
};
