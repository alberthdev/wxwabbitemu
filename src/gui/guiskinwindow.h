#ifndef GUISKINWINDOW_H
#define GUISKINWINDOW_H

#include <wx/wx.h>
#include "calc.h"

class SkinWindow : public wxWindow
{
private:
	LPCALC lpCalc;
	void OnPaint(wxPaintEvent &event);
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnLeftButtonUp(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void FinalizeButtons();
protected:
	DECLARE_EVENT_TABLE()
public:
	SkinWindow(wxFrame *parent, LPCALC lpCalc);
};

#endif