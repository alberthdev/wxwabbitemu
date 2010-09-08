#ifndef GUILCD_WX_H
#define GUILCD_WX_H

#include <wx/image.h>
#include <wx/frame.h>
#include <wx/wx.h>
#include "lcd.h"

class MyLCD: public wxWindow
{
public:
    MyLCD();
	wxWindow *frameLCD;
	void OnPaint(wxPaintEvent& event);
protected:
	
private:
	void PaintLCD(wxWindow *, wxPaintDC *);
};

#endif