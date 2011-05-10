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
MyLCD::MyLCD(int curslot)
	: wxWindow(calcs[curslot].wxFrame, ID_LCD, wxPoint(0,0),
		wxSize(calcs[curslot].cpu.pio.lcd->width * calcs[curslot].Scale, 64 * calcs[curslot].Scale)) {
	this->slot = curslot;
	int scale = calcs[curslot].Scale;
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(MyLCD::OnPaint));
	this->Connect(wxID_ANY, wxEVT_KEY_DOWN, (wxObjectEventFunction) &MyLCD::OnKeyDown);
	this->Connect(wxID_ANY, wxEVT_KEY_UP, (wxObjectEventFunction) &MyLCD::OnKeyUp);
	this->SetDropTarget(new DnDFile(this));
	int i;
#define LCD_HIGH	255
	for (i = 0; i <= MAX_SHADES; i++) {
		redColors[i] = (0x9E*(256-(LCD_HIGH/MAX_SHADES)*i))/255;
		greenColors[i] = (0xAB*(256-(LCD_HIGH/MAX_SHADES)*i))/255;
		blueColors[i] = (0x88*(256-(LCD_HIGH/MAX_SHADES)*i))/255;
	}
}

void MyLCD::OnKeyDown(wxKeyEvent& event)
{
	calcs[slot].wxFrame->OnKeyDown(event);
}

void MyLCD::OnKeyUp(wxKeyEvent& event)
{
	calcs[slot].wxFrame->OnKeyUp(event);
}

void MyLCD::OnPaint(wxPaintEvent& event)
{
	wxPaintDC *dc = new wxPaintDC(this);
	PaintLCD(this, dc);
	LCD_t *lcd = calcs[slot].cpu.pio.lcd;
	wxStatusBar *wxStatus = calcs[slot].wxFrame->GetStatusBar();
	if (wxStatus) {
		if (clock() > calcs[slot].sb_refresh + CLOCKS_PER_SEC / 2) {
			wxString sz_status;
			if (lcd->active)
				sz_status.sprintf(wxT("FPS: %0.2lf"), lcd->ufps);
			else
				sz_status.sprintf(wxT("FPS: -"));
			wxStatus->SetStatusText(sz_status, 0);
			calcs[slot].sb_refresh = clock();
		}
	}
	delete dc;
}

void MyLCD::PaintLCD(wxWindow *window, wxPaintDC *wxDCDest)
{
	unsigned char *screen;
	LCD_t *lcd = calcs[slot].cpu.pio.lcd;
	wxSize rc = calcs[slot].wxFrame->GetClientSize();			//GetClientRect(calcs[this->slot].hwndLCD, &rc);
	int draw_width = lcd->width * calcs[slot].Scale;
	int draw_height = 64 * calcs[slot].Scale;
	int scale = calcs[slot].Scale;
	wxPoint drawPoint((rc.GetWidth() - draw_width) / 2, 0);
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
		wxBitmap bmpBuf(screenImage.Scale(128 * scale, 64 * scale).Size(rc, wxPoint(0,0)));
		wxMemDC.SelectObject(bmpBuf);
		//draw drag panes
		/*if (calcs[this->slot].do_drag == TRUE) {

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
		wxDCDest->Blit(drawPoint.x, drawPoint.y, draw_width, draw_height, &wxMemDC, 0, 0);
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
		/*if (lcd->width * calcs[this->slot].Scale != (rc.right - rc.left))
			SetStretchBltMode(hdc, HALFTONE);
		else
			SetStretchBltMode(hdc, BLACKONWHITE);*/
		wxImage screenImage(128, 64, rgb_data, true);
		wxBitmap bmpBuf(screenImage.Scale(128 * scale, 64 * scale).Size(rc, wxPoint(0,0)));
		wxMemDC.SelectObject(bmpBuf);
		//if were dragging something we will draw these nice panes
		/*BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 160;
		bf.AlphaFormat = 0;

		if (calcs[this->slot].do_drag == TRUE) {

			hdcOverlay = DrawDragPanes(hwnd, hdcDest, 0);

			if (AlphaBlend(	hdc, 0, 0, rc.right, rc.bottom,
						hdcOverlay, 0, 0, rc.right, rc.bottom,
						bf ) == FALSE) printf("alpha blend 1 failed\n");

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
				calcs[this->slot].hdcSkin, calcs[this->slot].rectLCD.left, calcs[this->slot].rectLCD.top,
				(rc.right - rc.left), 128,
					bf )  == FALSE){
				//printf("alpha blend 2 failed\n");
				alphablendfail++;
			}
		}*/

		//finally copy up the screen image
		wxDCDest->Blit(drawPoint.x, drawPoint.y, draw_width, draw_height, &wxMemDC, 0, 0);
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
	
	wxFileDialog dialog(calcs[slot].wxLCD, wxT("Wabbitemu Save State"),
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
