#ifndef _ARCH_I386_KEYBOARD_H
#define _ARCH_I386_KEYBOARD_H

enum KEYBOARD_LED {
	LED_SCROLL_LOCK = 1,
	LED_NUM_LOCK = 2,
	LED_CAPS_LOCK = 4
};

void init_keyboard ();

int keyboard_get_leds ();

void keyboard_set_leds (int value);

struct KeyInfo keyboard_read_key ();

bool keyboard_try_read_key ();

int getc ();

enum Keys {
	KEY_UNKNOWN = 0x00,

	KEY_ESCAPE = 0x00,
	
	KEY_F1 = 0x3B,
	KEY_F2 = 0x3C,
	KEY_F3 = 0x3D,
	KEY_F4 = 0x3E,
	KEY_F5 = 0x3F,
	KEY_F6 = 0x40,
	KEY_F7 = 0x41,
	KEY_F8 = 0x42,
	KEY_F9 = 0x43,
	KEY_F10 = 0x44,
	KEY_F11 = 0x57,
	KEY_F12 = 0x58,
	
	KEY_PRINT_SCREEN = 0xE02AE037,
	//KEY_SCROLL_LOCK = 0x46,
	KEY_PAUSE_BREAK = 0xE11D45E1,
	
	KEY_VOLUME_MUTE = 0xE020,
	KEY_VOLUME_DOWN = 0xE02E,
	KEY_VOLUME_UP = 0xE030,
	
	KEY_D0 = 0x0B,
	KEY_D1 = 0x02,
	KEY_D2 = 0x03,
	KEY_D3 = 0x04,
	KEY_D4 = 0x05,
	KEY_D5 = 0x06,
	KEY_D6 = 0x07,
	KEY_D7 = 0x08,
	KEY_D8 = 0x09,
	KEY_D9 = 0x0A,
	
	KEY_BACKSPACE = 0x0E,
	
	KEY_A = 0x1E,
	KEY_B = 0x30,
	KEY_C = 0x2E,
	KEY_D = 0x20,
	KEY_E = 0x12,
	KEY_F = 0x21,
	KEY_G = 0x22,
	KEY_H = 0x23,
	KEY_I = 0x17,
	KEY_J = 0x24,
	KEY_K = 0x25,
	KEY_L = 0x26,
	KEY_M = 0x32,
	KEY_N = 0x31,
	KEY_O = 0x18,
	KEY_P = 0x19,
	KEY_Q = 0x10,
	KEY_R = 0x13,
	KEY_S = 0x1F,
	KEY_T = 0x14,
	KEY_U = 0x16,
	KEY_V = 0x2F,
	KEY_W = 0x11,
	KEY_X = 0x2D,
	KEY_Y = 0x15,
	KEY_Z = 0x2C,
	
	KEY_INSERT = 0xE052,
	KEY_DELETE = 0xE053,
	KEY_HOME = 0xE047,
	KEY_END = 0xE04F,
	KEY_PAGE_UP = 0xE049,
	KEY_PAGE_DOWN = 0xE051,
	
	KEY_UP_ARROW = 0xE048,
	KEY_LEFT_ARROW = 0xE04B,
	KEY_DOWN_ARROW = 0xE050,
	KEY_RIGHT_ARROW = 0xE04D,
	
	KEY_SPACE = 0x39,
	KEY_TAB = 0x0F,
	
	KEY_NUMPAD0 = 0x52,
	KEY_NUMPAD1 = 0x4F,
	KEY_NUMPAD2 = 0x50,
	KEY_NUMPAD3 = 0x51,
	KEY_NUMPAD4 = 0x4B,
	KEY_NUMPAD5 = 0x4C,
	KEY_NUMPAD6 = 0x4D,
	KEY_NUMPAD7 = 0x47,
	KEY_NUMPAD8 = 0x48,
	KEY_NUMPAD9 = 0x49,
	KEY_NUMPAD_ENTER = 0xE01C,
	
	KEY_CLEAR = 0xE04C,
	KEY_DECIMAL = 0x53,
	KEY_DIVIDE = 0xE035,
	KEY_MULTIPLY = 0x37,
	KEY_SUBTRACT = 0x4A,
	KEY_ADD = 0x4E,
	KEY_ENTER = 0x1C,
	
	KEY_LEFT_WINDOWS = 0xE05B,
	KEY_RIGHT_WINDOWS = 0xE05C,
	
	KEY_OEM1 = 0x27,
	KEY_OEM2 = 0x35,
	KEY_OEM3 = 0x29,
	KEY_OEM4 = 0x1A,
	KEY_OEM5 = 0x2B,
	KEY_OEM6 = 0x1B,
	KEY_OEM7 = 0x28,
	KEY_OEM8 = 0x29,
	KEY_OEM102 = 0x2B,
	KEY_OEM_COMMA = 0x33,
	KEY_OEM_MINUS = 0x0C,
	KEY_OEM_PERIOD = 0x34,
	KEY_OEM_PLUS = 0x0D,
	
	KEY_BROWSER_BACK = 0xE03A,
	KEY_BROWSER_FAVORITES = 0xE066,
	KEY_BROWSER_FORWARD = 0xE069,
	KEY_BROWSER_HOME = 0xE032,
	KEY_BROWSER_REFRESH = 0xE067,
	KEY_BROWSER_SEARCH = 0xE065,
	KEY_BROWSER_STOP = 0xE068,
	KEY_LAUNCH_APP1 = 0xE06B,
	KEY_LAUNCH_APP2 = 0xE021,
	KEY_LAUNCH_MAIL = 0xE06C,
	KEY_LAUNCH_MEDIA_SELECT = 0xE06D,
	KEY_MEDIA_NEXT = 0xE019,
	KEY_MEDIA_PLAY = 0xE022,
	KEY_MEDIA_PREVIOUS = 0xE010,
	KEY_MEDIA_STOP = 0xE024,
	KEY_PLAY = 0xE022,
	
	KEY_POWER = 0xE05E,
	KEY_SLEEP = 0xE05F,
	KEY_WAKE = 0xE063,
	
	KEY_OTHER
};

enum KeyModifiers {
	KM_NONE = 0,
	KM_ALT = 1,
	KM_SHIFT = 2,
	KM_CONTROL = 3
};

int keyboard_get_modifiers ();

struct KeyInfo {
	char Char;
	int Key;
	int Modifiers;
};

#endif
