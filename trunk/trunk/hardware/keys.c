#include "keys.h"
#include <wx/wx.h>


static keyprog_t keygrps[] = {
	{ 'A' , 5 , 6 },
	{ 'B' , 4 , 6 },
	{ 'C' , 3 , 6 },
	{ 'D' , 5 , 5 },
	{ 'E' , 4 , 5 },
	{ 'F' , 3 , 5 },
	{ 'G' , 2 , 5 },
	{ 'H' , 1 , 5 },
	{ 'I' , 5 , 4 },
	{ 'J' , 4 , 4 },
	{ 'K' , 3 , 4 },
	{ 'L' , 2 , 4 },
	{ 'M' , 1 , 4 },
	{ 'N' , 5 , 3 },
	{ 'O' , 4 , 3 },
	{ 'P' , 3 , 3 },
	{ 'Q' , 2 , 3 },
	{ 'R' , 1 , 3 },
	{ 'S' , 5 , 2 },
	{ 'T' , 4 , 2 },
	{ 'U' , 3 , 2 },
	{ 'V' , 2 , 2 },
	{ 'W' , 1 , 2 },
	{ 'X' , 5 , 1 },
	{ 'Y' , 4 , 1 },
	{ 'Z' , 3 , 1 },
	{ ' ' , 4 , 0 },
	{ WXK_DOWN , 0 , 0 },
	{ WXK_LEFT , 0 , 1 },
	{ WXK_RIGHT , 0 , 2 },
	{ WXK_UP , 0 , 3 },
	{ '0' , 4 , 0 },
	{ '1' , 4 , 1 },
	{ '2' , 3 , 1 },
	{ '3' , 2 , 1 },
	{ '4' , 4 , 2 },
	{ '5' , 3 , 2 },
	{ '6' , 2 , 2 },
	{ '7' , 4 , 3 },
	{ '8' , 3 , 3 },
	{ '9' , 2 , 3 },
	
	{ WXK_RETURN , 1 , 0 },
	{ WXK_DECIMAL , 3 , 0 },
	//{ VKF_COMMA , 4 , 4 },
	{ WXK_ADD , 1 , 1 },
	{ WXK_SUBTRACT , 1 , 2 },
	{ WXK_MULTIPLY , 1 , 3 },
	{ WXK_DIVIDE , 1 , 4 },
	//{ VKF_LBRACKET , 3 , 4 },
	//{ VKF_RBRACKET , 2 , 4 },
	{ WXK_F1 , 6 , 4 },
	{ WXK_F2 , 6 , 3 },
	{ WXK_F3 , 6 , 2 },
	{ WXK_F4 , 6 , 1 },
	{ WXK_F5 , 6 , 0 },
	{ WXK_ESCAPE , 6 , 6 },
	{ WXK_SHIFT , 6 , 5 },			// l shift
	{ WXK_CONTROL, 5 , 7 },			// l control
	//{ VK_RSHIFT , 1 , 6 },
	//{ WXF_MINUS , 2 , 0 },
	{ WXK_NUMPAD_EQUAL , 4 , 7 },
	//{ VK_PRIOR , 4 , 6 },
	//{ VK_NEXT , 3 , 6 },
	{ WXK_INSERT , 2 , 6 },
	{ WXK_DELETE , 6 , 7 },
	{ WXK_HOME , 5 , 6 },
	{ WXK_END , 3 , 7 },
	//{ VK_NUMPAD0 , 4 , 0 },
	{ WXK_NUMPAD1 , 4 , 1 },
	{ WXK_NUMPAD2 , 3 , 1 },
	{ WXK_NUMPAD3 , 2 , 1 },
	{ WXK_NUMPAD4 , 4 , 2 },
	{ WXK_NUMPAD5 , 3 , 2 },
	{ WXK_NUMPAD6 , 2 , 2 },
	{ WXK_NUMPAD7 , 4 , 3 },
	{ WXK_NUMPAD8 , 3 , 3 },
	{ WXK_NUMPAD9 , 2 , 3 },
	{ WXK_DECIMAL , 3 , 0 },
	//{ VK_OEM_2, 2, 0 },
	{ WXK_F12, 20, 0}
};
	
keypad_t *keypad_init(CPU_t *cpu) {
	keypad_t *keypad;
	int b,i;
	
	keypad = (keypad_t *) malloc(sizeof(keypad_t));
	if (!keypad) {
		printf("Couldn't allocate mem for keypad\n");
		return NULL;
	}

	for(b=0;b<8;b++) {
		for(i=0;i<8;i++) {
			keypad->keys[b][i]=0;
		}
	}
	keypad->on_pressed = 0;
	keypad->group = 0;
	return keypad;
}

void keypad(CPU_t *cpu, device_t *dev) {
	keypad_t *keypad = (keypad_t *) dev->aux;

	if (cpu->input) {
		int i,group,keybit;
		unsigned char result=0;
		unsigned char keymap[8] = {0,0,0,0,0,0,0,0};
		unsigned char keymapbug[8] = {0,0,0,0,0,0,0,0};

		for (group = 0; group < 7; group++) {
			for (keybit = 0; keybit < 8; keybit++) {
				if (keypad->keys[group][keybit]) {
					keymap[group] |= (1<<keybit);
				}
			}
		}
		
		for (group = 0; group < 7; group++) {
			for (i = 0; i < 7; i++) {
				if (keymap[group] & keymap[i]) {
					keymapbug[group] |= keymap[group]|keymap[i];
				}
			}
		}
		
		for (group = 0; group < 7; group++) {
			if (keypad->group & (1<<group)) {
				result |= keymapbug[group];
			}
		}
	

		cpu->bus = ~result;
		cpu->input = FALSE;
	} else if (cpu->output) {
		unsigned char group = ~cpu->bus;
		if (group != 0) keypad->group = group;
		else keypad->group = 0;
		cpu->output = FALSE;
	}	
}

keyprog_t *keypad_key_press(CPU_t *cpu, unsigned int vk) {
	int i;
	keypad_t * keypad = cpu->pio.keypad;

#ifndef WINVER
	if (vk >= 'a' && vk <= 'z') vk += 'A' - 'a';
#endif
	for(i=0; i < NumElm(keygrps); i++) {
		if (keygrps[i].vk == vk) {
			if (keygrps[i].group == 20) {
				keypad->on_pressed |= KEY_KEYBOARDPRESS;
			} else {
				keypad->keys[keygrps[i].group][keygrps[i].bit] |= KEY_KEYBOARDPRESS;
			}
			return &keygrps[i];
		}
	}	
	return NULL;
}

keyprog_t *keypad_key_release(CPU_t *cpu, unsigned int vk) {
	int i;
	keypad_t * keypad = cpu->pio.keypad;
	
#ifndef WINVER
	if (vk >= 'a' && vk <= 'z') vk += 'A' - 'a';
#endif
	
	for(i=0; i < NumElm(keygrps); i++) {
		if (keygrps[i].vk == vk) {
			if (keygrps[i].group == 20) {
				keypad->on_pressed &= (~KEY_KEYBOARDPRESS);
			} else {			
				keypad->keys[keygrps[i].group][keygrps[i].bit] &= (~KEY_KEYBOARDPRESS);
			}
			return &keygrps[i];
		}
	}
	return NULL;
}

