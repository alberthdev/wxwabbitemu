#include "guilcd_wx.h"
#include "calc.h"
#include "core.h"


enum {
	ID_LCD,
};

MyLCD::MyLCD() {
	int scale = calcs[gslot].Scale;
	const wxPoint startPos(0,0);
	const wxSize startSize(calcs[gslot].cpu.pio.lcd->width* scale, 64*scale);
	frameLCD = new wxWindow(calcs[gslot].wxFrame->frameMain, ID_LCD, startPos, startSize);
	frameLCD->Connect(ID_LCD, wxEVT_PAINT, (wxObjectEventFunction) &MyLCD::OnPaint);
}

void MyLCD::OnPaint(wxPaintEvent& event)
{
	//wxMessageBox(wxT("Paint"), wxT("OnPaint"), wxOK, NULL);
	if (frameLCD == NULL)
		return;
	wxPaintDC dc(this);
	PaintLCD(frameLCD, &dc);
	//TODO: add in fps counter in the status bar
	//LCD_t *lcd = calcs[gslot].cpu.pio.lcd;
}

void MyLCD::PaintLCD(wxWindow *window, wxPaintDC *wxDCDest)
{
	unsigned char *screen;
	LCD_t *lcd = calcs[gslot].cpu.pio.lcd;
	wxPoint drawPoint(0,0);
	wxSize rc = wxSize(192,128);//window->GetClientSize();			//GetClientRect(calcs[gslot].hwndLCD, &rc);

	//wxMessageBox(wxT("Paint"), wxT("OnPaint"), wxOK, NULL);
	//wxClientDC wxMyDC(window), wxDCOverlay;
	wxMemoryDC wxMemDC;
	/*if (!wxMyDC.IsOk()) {
		printf("Creating buffer DC failed\n");
		return;
	}*/
	if (lcd->active == false) {
		char lcd_data[128*64];
		memset(lcd_data, 0xFF0000, sizeof(lcd_data));
		wxBitmap bmpBuf((const char* ) &lcd_data, wxBITMAP_TYPE_XBM, rc.GetWidth(), rc.GetHeight());
		/*if (wxBitmap.Create(lcd_data, wxBitmap StretchDIBits(
			hdc,
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			0, 0, lcd->width, 64,
			lcd_data,
			bi,
			DIB_RGB_COLORS,
			SRCCOPY) == 0) {

			printf("error in SetDIBitsToDevice\n");
		}*/
		wxMemDC.SelectObject(bmpBuf);

		//draw drag panes
		/*if (calcs[gslot].do_drag == TRUE) {

			hdcOverlay = DrawDragPanes(hwnd, hdcDest, 0);
			BLENDFUNCTION bf;
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = 160;
			bf.AlphaFormat = 0;
			if (AlphaBlend(	hdc, 0, 0, rc.right, rc.bottom,
						hdcOverlay, 0, 0, rc.right, rc.bottom,
						bf ) == FALSE) printf("alpha blend 1 failed\n");

			DeleteDC(hdcOverlay);

		}*/

		//copy to the screen
		wxDCDest->Blit(drawPoint.x, drawPoint.y, rc.GetWidth(), rc.GetHeight(), &wxMemDC, 0, 0);
		/*if (BitBlt(	hdcDest, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			hdc, 0, 0, SRCCOPY ) == FALSE) printf("BitBlt failed\n");*/

	} else {
		screen = LCD_image( calcs[gslot].cpu.pio.lcd ) ;
		//screen = GIFGREYLCD();

		//this is for the 86, so it doesnt look like complete shit
		/*if (lcd->width * calcs[gslot].Scale != (rc.right - rc.left))
			SetStretchBltMode(hdc, HALFTONE);
		else
			SetStretchBltMode(hdc, BLACKONWHITE);*/
		wxBitmap bmpBuf((const char* ) &screen, wxBITMAP_TYPE_XBM, rc.GetWidth(), rc.GetHeight());
		/*if (StretchDIBits(	hdc,
							rc.left, rc.top, rc.right - rc.left,  rc.bottom - rc.top,
							0, 0, lcd->width, 64,
							screen,
							bi,
							DIB_RGB_COLORS,
							SRCCOPY) == 0) {
							printf("error in SetDIBitsToDevice\n");
						}*/


		//if were dragging something we will draw these nice panes
		/*BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 160;
		bf.AlphaFormat = 0;

		if (calcs[gslot].do_drag == TRUE) {

			hdcOverlay = DrawDragPanes(hwnd, hdcDest, 0);

			if (AlphaBlend(	hdc, 0, 0, rc.right, rc.bottom,
						hdcOverlay, 0, 0, rc.right, rc.bottom,
						bf ) == FALSE) printf("alpha blend 1 failed\n");

			DeleteDC(hdcOverlay);

		}*/


		//if were sending something we can draw the sending shtuff
		/*if (calcs[gslot].send == TRUE) {
			bf.SourceConstantAlpha = 192;
			hdcOverlay = DrawSending(hwnd, hdcDest);

			if (AlphaBlend(	hdc, 0, 0, rc.right, rc.bottom,
						hdcOverlay, 0, 0, rc.right, rc.bottom,
						bf ) == FALSE) printf("alpha blend send failed\n");

			DeleteDC(hdcOverlay);
		}*/

		//this alphablends the skin to the screen making it look nice
		/*bf.SourceConstantAlpha = 108;

		POINT pt;
		pt.x = rc.left;
		pt.y = rc.top;
		ClientToScreen(hwnd, &pt);
		ScreenToClient(GetParent(hwnd), &pt);

		if (alphablendfail<100 && lcd->width != 138) {
			if (AlphaBlend(	hdc, rc.left, rc.top, rc.right,  rc.bottom,
				calcs[gslot].hdcSkin, calcs[gslot].rectLCD.left, calcs[gslot].rectLCD.top,
				(rc.right - rc.left), 128,
					bf )  == FALSE){
				//printf("alpha blend 2 failed\n");
				alphablendfail++;
			}
		}*/

		//finally copy up the screen image
		wxDCDest->Blit(drawPoint.x, drawPoint.y, rc.GetWidth(), rc.GetHeight(), &wxMemDC, 0, 0);
		/*if (BitBlt(	hdcDest, rc.left, rc.top, rc.right - rc.left,  rc.bottom - rc.top,
			hdc, 0, 0, SRCCOPY ) == FALSE) printf("Bit blt failed\n");*/

	}
	//delete wxBitmap;				//DeleteObject(bmpBuf);
	//delete &wxMemDC; 				//DeleteDC(hdc);
}