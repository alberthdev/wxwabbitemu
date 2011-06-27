#ifndef GUILCD_WX_H
#define GUILCD_WX_H

#include <wx/image.h>
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/palette.h>
#include "lcd.h"
#include <stdlib.h>

void SaveStateDialog(int slot);
#define MAX_SHADES 255
class MyLCD: public wxWindow
{
public:
    MyLCD(int curslot);

	void PaintNow();
protected:
	
private:
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void PaintLCD(wxWindow *, wxPaintDC *);
	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnLeftButtonUp(wxMouseEvent& event);

	int slot;
};

#endif
