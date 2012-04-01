#include "guiskinwindow.h"

BEGIN_EVENT_TABLE(SkinWindow, wxWindow)
	EVT_PAINT(SkinWindow::OnPaint)
	EVT_LEFT_DOWN(SkinWindow::OnLeftButtonDown)
	EVT_LEFT_UP(SkinWindow::OnLeftButtonUp)
	EVT_KEY_DOWN(SkinWindow::OnKeyDown)
	EVT_KEY_UP(SkinWindow::OnKeyUp)
END_EVENT_TABLE()

SkinWindow::SkinWindow(wxFrame *parent, LPCALC lpCalc) : wxWindow(parent, wxID_ANY)
{
	this->lpCalc = lpCalc;
}

void SkinWindow::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
	if (lpCalc->SkinEnabled) {
		dc.DrawBitmap(lpCalc->calcSkin, 0, 0, true);
	}
}

void SkinWindow::OnLeftButtonDown(wxMouseEvent& event)
{
	event.Skip(true);
	static wxPoint pt;
	keypad_t *kp = lpCalc->cpu.pio.keypad;

	//CopySkinToButtons();
	//CaptureMouse();
	pt.x	= event.GetX();
	pt.y	= event.GetY();
	/*if (lpCalc->bCutout) {
		pt.y += GetSystemMetrics(SM_CYCAPTION);
		pt.x += GetSystemMetrics(SM_CXSIZEFRAME);
	}*/
	for(int group = 0; group < 7; group++) {
		for(int bit = 0; bit < 8; bit++) {
			kp->keys[group][bit] &= (~KEY_MOUSEPRESS);
		}
	}

	lpCalc->cpu.pio.keypad->on_pressed &= ~KEY_MOUSEPRESS;

	/*if (!event.LeftDown()) {
		//FinalizeButtons(lpCalc);
		return;
	}*/

	if (lpCalc->keymap.GetRed(pt.x, pt.y) == 0xFF) {
		//FinalizeButtons(lpCalc);
		return;
	}

	int green = lpCalc->keymap.GetGreen(pt.x, pt.y);
	int blue = lpCalc->keymap.GetBlue(pt.x, pt.y);
	if ((green >> 4) == 0x05 && (blue >> 4) == 0x00)
	{
		lpCalc->cpu.pio.keypad->on_pressed |= KEY_MOUSEPRESS;
	} else {
		kp->keys[green >> 4][blue >> 4] |= KEY_MOUSEPRESS;
		if ((kp->keys[green >> 4][blue >> 4] & KEY_STATEDOWN) == 0) {
			//DrawButtonState(lpCalc, lpCalc->hdcButtons, lpCalc->hdcKeymap, &pt, DBS_DOWN | DBS_PRESS);
			kp->keys[green >> 4][blue >> 4] |= KEY_STATEDOWN;
		}
	}
}

void SkinWindow::OnLeftButtonUp(wxMouseEvent& event)
{
	event.Skip(true);
	static wxPoint pt;
	keypad_t *kp = lpCalc->cpu.pio.keypad;

	//ReleaseMouse();

	for(int group = 0; group < 7; group++) {
		for(int bit = 0; bit < 8; bit++) {
			kp->keys[group][bit] &= ~(KEY_MOUSEPRESS | KEY_STATEDOWN);
		}
	}

	lpCalc->cpu.pio.keypad->on_pressed &= ~KEY_MOUSEPRESS;
}

void SkinWindow::OnKeyDown(wxKeyEvent& event)
{
	int keycode = event.GetKeyCode();
	if (keycode == WXK_SHIFT) {
		wxUint32 raw = event.GetRawKeyCode();
		if (raw == 65505) {
			keycode = WXK_LSHIFT;
		} else {
			keycode = WXK_RSHIFT;
		}
	}

	keyprog_t *kp = keypad_key_press(&lpCalc->cpu, keycode);
	if (kp) {
		if ((lpCalc->cpu.pio.keypad->keys[kp->group][kp->bit] & KEY_STATEDOWN) == 0) {
			lpCalc->cpu.pio.keypad->keys[kp->group][kp->bit] |= KEY_STATEDOWN;
			this->Update();
			FinalizeButtons();
		}
	}
}

void SkinWindow::OnKeyUp(wxKeyEvent& event)
{
	int key = event.GetKeyCode();
	if (key == WXK_SHIFT) {
		keypad_key_release(&lpCalc->cpu, WXK_LSHIFT);
		keypad_key_release(&lpCalc->cpu, WXK_RSHIFT);
	} else {
		keypad_key_release(&lpCalc->cpu, key);
	}
	FinalizeButtons();
}

void SkinWindow::FinalizeButtons() {
	int group, bit;
	keypad_t *kp = lpCalc->cpu.pio.keypad;
	for(group = 0; group < 7; group++) {
		for(bit = 0; bit < 8; bit++) {
			if ((kp->keys[group][bit] & KEY_STATEDOWN) &&
				((kp->keys[group][bit] & KEY_MOUSEPRESS) == 0) &&
				((kp->keys[group][bit] & KEY_KEYBOARDPRESS) == 0)) {
					kp->keys[group][bit] &= (~KEY_STATEDOWN);
			}
		}
	}
}