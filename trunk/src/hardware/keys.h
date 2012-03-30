#ifndef KEYS_H
#define KEYS_H
#include "core.h"

typedef struct keypad {
	unsigned char group;
	unsigned char keys[8][8], on_pressed;
	unsigned long long last_pressed[8][8], on_last_pressed;
} keypad_t;

typedef struct KEYPROG {
	int vk;
	int group;
	int bit;
} keyprog_t;

//shhh..Redefine for sake of ease
#define VKF_EQUAL	0xBB
#define VKF_COMMA	0xBC
#define VKF_MINUS	0xBD
#define VKF_PERIOD	0xBE
#define VKF_LBRACKET 0xDB
#define VKF_RBRACKET 0xDD
#define VKF_QUOTE	0xDE
#define WXK_LSHIFT WXK_SPECIAL20+1
#define WXK_RSHIFT WXK_LSHIFT+1


keypad_t *keypad_init(CPU_t*);
void keypad(CPU_t *, device_t *);

keyprog_t *keypad_key_press(CPU_t*, unsigned int vk, BOOL *changed = NULL);
keyprog_t *keypad_key_release(CPU_t*, unsigned int vk);
void keypad_press(CPU_t *cpu, int group, int bit);
void keypad_release(CPU_t *cpu, int group, int bit);

#ifdef WINVER
//used by the debugger to fix stuck keys
void keypad_vk_release(HWND hwnd, int group, int bit);
#endif

#define KEY_VALUE_MASK		(0x0F)

#define KEY_KEYBOARDPRESS	0x01
#define KEY_MOUSEPRESS		0x02
#define KEY_LOCKPRESS		0x04
#define KEY_FALSEPRESS		0x08
#define KEY_STATEDOWN		0x10

#define KEYGROUP_ON			0x05
#define KEYBIT_ON			0x00

#define NumElm(array) (sizeof (array) / sizeof ((array)[0]))

#endif /*#ifndef KEYS_H*/
