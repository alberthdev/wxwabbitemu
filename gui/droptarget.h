#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/dnd.h>

class DnDFile : public wxFileDropTarget
{
public:
    DnDFile(wxWindow *pOwner) { m_pOwner = pOwner; }

    virtual bool OnDropFiles(wxCoord x, wxCoord y,
                             const wxArrayString& filenames);

private:
    wxWindow *m_pOwner;
};