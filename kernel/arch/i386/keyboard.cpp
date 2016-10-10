#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arch/i386/portio.h>
#include <arch/i386/keyboard.h>
#include <arch/i386/pic.h>

struct queue_node {
	struct KeyInfo key;
	struct queue_node *next;
};

struct queue_node *front;
struct queue_node *rear;

static void enqueue_key (struct KeyInfo item) {
	struct queue_node *temp = (struct queue_node*) malloc (sizeof (struct queue_node));
	temp->key = item;
	temp->next = nullptr;
	if (front == nullptr && rear == nullptr) {
		front = rear = temp;
		return;
	}
	rear->next = temp;
	rear = temp;
}

static int kb_led_status;
static int kb_modifiers;

static void keyboard_wait(void) {
	while (inb (0x64) & 2);
}

static void keyboard_update_leds () {
	outb (0x60, 0xED);
	keyboard_wait ();
	outb (0x60, kb_led_status);
	keyboard_wait ();
}

int keyboard_get_leds () {
	return kb_led_status;
}

void keyboard_set_leds (int value) {
	kb_led_status = value;
	keyboard_update_leds ();
}

int keyboard_get_modifiers () {
	return kb_modifiers;
}

struct KeyInfo keyboard_read_key () {
	while (front == nullptr) {
		asm volatile ("hlt");
	}
	struct KeyInfo res;
	if (front == nullptr) {
		struct KeyInfo res;
		memset (&res, 0, sizeof (KeyInfo));
		return res;
	}
	res = front->key;
	if (front == rear) {
		front = rear = nullptr;
	}
	else {
		front = front->next;
	}
	return res;
}

bool keyboard_try_read_key (struct KeyInfo *res) {
	if (front == nullptr) {
		return false;
	}
	*res = front->key;
	if (front == rear) {
		front = rear = nullptr;
	}
	else {
		front = front->next;
	}
	return true;
}

static char const *chsL = 	"  1234567890-= \t"
							"qwertyuiop[]\n "
							"asdfghjkl;'` "
							"\\zxcvbnm,./"
							" *   "
							"          " // F1-10
							"  789-456+1230."
							;

static char const *chsU = 	"  !@#$%^&*()_+ \t"
							"QWERTYUIOP{}\n "
							"ASDFGHJKL:\"~ "
							"|ZXCVBNM<>?"
							" *   "
							"          " // F1-10
							"     -   +    ."
							;

static bool keyboard_get_key (int key, struct KeyInfo *keyInfo) {
	keyInfo->Modifiers = kb_modifiers;
	if ((!(kb_led_status & LED_NUM_LOCK) || ((kb_led_status & LED_NUM_LOCK) && (kb_modifiers & KM_SHIFT))) && key >= 0x48 && key <= 0x52 && key != 0x4A && key != 0x4E) {
		key |= 0xE000;
	}
	keyInfo->Key = key;
	keyInfo->Char = '\0';
	if (key == 0xE035 || key == 0xE01C) key ^= 0xE000;
	switch (key) {
		case KEY_BACKSPACE:
				keyInfo->Char = '\b';
			break;
		default:
			if (key <= 0x53) {
				keyInfo->Char = (((kb_modifiers & KM_SHIFT) != 0) ^ ((kb_led_status & LED_CAPS_LOCK) != 0) ? chsU : chsL)[key];
			}
			break;
	}
	if (keyInfo->Char == ' ' && key != 0x39) {
		keyInfo->Char = '\0';
	}
	return true;
}

static void handle_scancode(int key, bool released) {
	if (key == 0xE0) printf("ext\n");
	switch (key) {
		case 0x1D: // Left CTRL
		case 0xE01D: // Right CTRL
			kb_modifiers = (kb_modifiers & ~KM_CONTROL) | (released ? 0 : KM_CONTROL);
			break;

		case 0x2A: // Left SHIFT
		case 0x36: // Right SHIFT
			kb_modifiers = (kb_modifiers & ~KM_SHIFT) | (released ? 0 : KM_SHIFT);
			break;

		case 0x38: // Left ALT
		case 0xE038:
			kb_modifiers = (kb_modifiers & ~KM_ALT) | (released ? 0 : KM_ALT);
			break;

		default:
			if ((kb_modifiers & KM_CONTROL) && (kb_modifiers & KM_ALT) && (key == 0x53 || key == 0xE053)) {
				// ctrl-alt-del
				// reboot?
				printf ("Detected Ctrl-Alt-Delete! Rebooting System...\n");
				unsigned char good = 0x02;
    			while (good & 0x02)
        			good = inb (0x64);
    			outb (0x64, 0xFE);
    			asm volatile ("hlt");
			}
			if (!released) {
				if (key == 0x3A) kb_led_status ^= LED_CAPS_LOCK;
				else if (key == 0x45) kb_led_status ^= LED_NUM_LOCK;
				else if (key == 0x46) kb_led_status ^= LED_SCROLL_LOCK;
				else {
					struct KeyInfo keyInfo;
					if (keyboard_get_key (key, &keyInfo)) {
						enqueue_key (keyInfo);
					}
				}
			}
			break;
	}
}

static bool waitExtended = false;
static int prntCount = 0;
static int pauseCount = 0;
static bool prntRel = false;

static void keyboard_handler (regs_t *ctx) {
  (void) ctx;

	int code = inb(0x60);
	if (pauseCount >= 1) {
		if (pauseCount == 7) {
			pauseCount = 0;
			handle_scancode (0xE11D45E1, false);
			handle_scancode (0xE11D45E1, true);
		}
		else pauseCount++;
		return;
	}
	if (prntCount >= 1) {
		if (prntCount == 5) {
			prntCount = 0;
			handle_scancode (0xE02AE037, prntRel);
			prntRel = false;
		}
		else prntCount++;
		return;
	}
	if (waitExtended) {
		code |= 0xE000;
		waitExtended = false;
	}
	if (code == 0xE0) {
		waitExtended = true;
		return;
	}
	else if (code == 0xE1) {
		pauseCount = 1;
		return;
	}
	bool released = code & 0x80;
	/*if (code == 0xE02A) {       <-- it makes page fault every time you press an extended key
		prntCount = 1;
		prntRel = false;
		return;
	}
	else if (code == 0xE0B7) {
		prntCount = 1;
		prntRel = true;
		return;
	}*/
	if (released) {
		code ^= 0x80;
	}
	handle_scancode (code, released);
}

void init_keyboard () {
	request_irq (1, keyboard_handler);
}

int getc () {
	struct KeyInfo k = keyboard_read_key ();
	int ch = k.Char;
	if (ch != 0 && ch != '\b')
		putchar (ch);
	return ch;
}

char *gets () {
    char *str = new char [256];
    memset (str, 0, 256);
    char *init = str;
    char c;
    do {
        if (c != 0) {
            if (c == '\b') {
                if (str - init > 0) {
                    *(--str) = '\0';
                    putchar ('\b');
                }
            }
            else
                *(str++) = c;
        }
    } while ((c = getc ()) != '\n');
    *(str++) = c;

    return init;
}
