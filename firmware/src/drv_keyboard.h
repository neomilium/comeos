#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <avr/io.h>

#include <stdint.h>


#define KEYBOARD_UP		0x08
#define KEYBOARD_DOWN		0x20
#define KEYBOARD_OK		0x04
#define KEYBOARD_RIGHT		0x80
#define KEYBOARD_LEFT		0x40

#define KEYBOARD_EVENT_NONE		0x00
#define KEYBOARD_EVENT_KEY_PRESSED	0x01
#define KEYBOARD_EVENT_KEY_RELEASED	0x02

typedef struct {
		uint8_t code;
		uint8_t data;
} keyboard_event_t;

void		keyboard_init(void);
void		keyboard_poll(void);
keyboard_event_t keyboard_get_event(void);

#endif
