#ifndef GUILCD_WX_H
#define GUILCD_WX_H

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/frame.h>
#include <wx/palette.h>
#include "calc.h"
#include "lcd.h"
#include <stdlib.h>

void SaveStateDialog(LPCALC lpCalc);
#define MAX_SHADES 255
class WabbitemuLCD: public wxWindow
{
public:
    WabbitemuLCD(wxFrame *mainFrame, LPCALC lpCalc);

	void PaintNow();
protected:
	DECLARE_EVENT_TABLE()
private:
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void PaintLCD(wxWindow *, wxPaintDC *);
	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnLeftButtonUp(wxMouseEvent& event);
	void FinalizeButtons();

	bool hasDrawnLCD;
	LPCALC lpCalc;
	wxFrame *mainFrame;
};

#endif
