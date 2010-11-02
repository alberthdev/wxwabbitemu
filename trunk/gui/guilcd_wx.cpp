#include "guilcd_wx.h"
#include "calc.h"
#include "core.h"
#include "droptarget.h"


enum {
	ID_LCD,
};

unsigned char redColors[MAX_SHADES+1];
unsigned char greenColors[MAX_SHADES+1];
unsigned char blueColors[MAX_SHADES+1];
MyLCD::MyLCD() {
	int scale = calcs[gslot].Scale;
	const wxPoint startPos(0,0);
	const wxSize startSize(calcs[gslot].cpu.pio.lcd->width* scale, 64*scale);
	frameLCD = new wxWindow(calcs[gslot].wxFrame->frameMain, ID_LCD, startPos, startSize);
	frameLCD->Connect(ID_LCD, wxEVT_PAINT, (wxObjectEventFunction) &MyLCD::OnPaint);
	frameLCD->Connect(wxID_ANY, wxEVT_KEY_DOWN, (wxObjectEventFunction) &MyFrame::OnKeyDown);
	frameLCD->Connect(wxID_ANY, wxEVT_KEY_UP, (wxObjectEventFunction) &MyFrame::OnKeyUp);
	frameLCD->SetDropTarget(new DnDFile(frameLCD));
	int i;
#define LCD_HIGH	255
	for (i = 0; i <= MAX_SHADES; i++) {
		redColors[i] = (0x9E*(256-(LCD_HIGH/MAX_SHADES)*i))/255;
		greenColors[i] = (0xAB*(256-(LCD_HIGH/MAX_SHADES)*i))/255;
		blueColors[i] = (0x88*(256-(LCD_HIGH/MAX_SHADES)*i))/255;
	}
}

void MyLCD::OnPaint(wxPaintEvent& event)
{
	if (frameLCD == NULL)
		return;
	wxPaintDC dc(this);
	PaintLCD(frameLCD, &dc);
	//TODO: add in fps counter in the status bar
	LCD_t *lcd = calcs[gslot].cpu.pio.lcd;
	wxStatusBar *wxStatus = calcs[gslot].wxFrame->frameMain->GetStatusBar();
	if (wxStatus) {
		if (clock() > calcs[gslot].sb_refresh + CLOCKS_PER_SEC/2) {
			wxString sz_status;
			if (lcd->active)
				sz_status.sprintf(wxT("FPS: %0.2lf"), lcd->ufps);
			else
				sz_status.sprintf(wxT("FPS: -"));
			wxStatus->SetStatusText(sz_status, 1);
			calcs[gslot].sb_refresh = clock();
		}
	}
}

void MyLCD::PaintLCD(wxWindow *window, wxPaintDC *wxDCDest)
{
	unsigned char *screen;
	LCD_t *lcd = calcs[gslot].cpu.pio.lcd;
	wxPoint drawPoint(32, 0);
	wxSize rc = wxSize(192,128);//window->GetClientSize();			//GetClientRect(calcs[gslot].hwndLCD, &rc);

	wxMemoryDC wxMemDC;	
	if (lcd->active == false) {
		unsigned char lcd_data[128*64];
		memset(lcd_data, 0, sizeof(lcd_data));
		unsigned char rgb_data[128*64*3];
		int i, j;
		for (i = j = 0; i < 128*64; i++, j+=3) {
			rgb_data[j] = redColors[lcd_data[i]];
			rgb_data[j+1] = greenColors[lcd_data[i]];
			rgb_data[j+2] = blueColors[lcd_data[i]];
		}
		wxImage screenImage(128, 64, rgb_data, true);
		wxBitmap bmpBuf(screenImage.Scale(256, 128).Size(rc, wxPoint(0,0)));
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
		wxMemDC.SelectObject(wxNullBitmap);

	} else {
		screen = LCD_image( calcs[gslot].cpu.pio.lcd ) ;
		unsigned char rgb_data[128*64*3];
		int i, j;
		for (i = j = 0; i < 128*64; i++, j+=3) {
			rgb_data[j] = redColors[screen[i]];
			rgb_data[j+1] = greenColors[screen[i]];
			rgb_data[j+2] = blueColors[screen[i]];
		}

		//this is for the 86, so it doesnt look like complete shit
		/*if (lcd->width * calcs[gslot].Scale != (rc.right - rc.left))
			SetStretchBltMode(hdc, HALFTONE);
		else
			SetStretchBltMode(hdc, BLACKONWHITE);*/
		wxImage screenImage(128, 64, rgb_data, true);
		wxBitmap bmpBuf(screenImage.Scale(256, 128).Size(rc, wxPoint(0,0)));
		wxMemDC.SelectObject(bmpBuf);
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
		wxDCDest->Blit(drawPoint.x, drawPoint.y, (rc.GetWidth() - 32) * 2, rc.GetHeight() *2, &wxMemDC, 0, 0);
		wxMemDC.SelectObject(wxNullBitmap);
		/*if (BitBlt(	hdcDest, rc.left, rc.top, rc.right - rc.left,  rc.bottom - rc.top,
			hdc, 0, 0, SRCCOPY ) == FALSE) printf("Bit blt failed\n");*/

	}
	//delete wxBitmap;				//DeleteObject(bmpBuf);
	//delete &wxMemDC; 				//DeleteDC(hdc);
}

void SaveStateDialog(int slot) {
	char *FileName;
	wxString lpstrFilter 	= wxT("\
Known File types ( *.sav; *.rom; *.bin) |*.sav;*.rom;*.bin|\
Save States  (*.sav)|*.sav|\
ROMS  (*.rom; .bin)|*.rom;*.bin|\
All Files (*.*)|*.*\0");
	
	wxFileDialog dialog(calcs[slot].wxLCD->frameLCD, wxT("Wabbitemu Save State"),
	wxT(""), wxT(""), lpstrFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dialog.ShowModal() != wxID_OK)
		return;
	FileName = wxStringToChar(dialog.GetPath());
	
	SAVESTATE_t* save = SaveSlot(slot);

	strcpy(save->author, "Default");
	save->comment[0] = '\0';
	WriteSave(FileName, save, false);
	//gui_savestate(save, FileName);

}