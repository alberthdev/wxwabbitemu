#ifndef GUILCD_WX_H
#define GUILCD_WX_H

#include <wx/image.h>
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/palette.h>
#include "lcd.h"
#include <stdlib.h>
#include "calc.h"

void SaveStateDialog(LPCALC lpCalc);
#define MAX_SHADES 255
class MyLCD: public wxWindow
{
public:
    MyLCD(wxFrame *mainFrame, LPCALC lpCalc);

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

	LPCALC lpCalc;
	wxFrame *mainFrame;
};

#endif
