#include "drv_keyboard.h"

#include "scheduler.h"

#define KEYBOARD_MASK					0xEC
#define KEYBOARD_PIN					PIND
#define KEYBOARD_PORT					PORTD
#define KEYBOARD_DDR					DDRD

#define KEYBOARD_KEY_REPEATER_MAX_PASS			5

static volatile uint8_t _keyboard_status = 0x00;

static volatile keyboard_event_t _keyboard_wanted_event;
static volatile keyboard_event_t _keyboard_previous_wanted_event;
static volatile uint8_t _keyboard_key_repeater_counter;

void
keyboard_init(void)
{
	KEYBOARD_DDR = 0x03;	/* Set keyboard port in input */
	KEYBOARD_PORT = KEYBOARD_MASK;	/* Set internal pull-up resistances */
	scheduler_add_hook_fct(keyboard_poll);
	_keyboard_status = KEYBOARD_PORT & KEYBOARD_MASK;
	_keyboard_key_repeater_counter = 0;
}

void
keyboard_poll(void)
{
	_keyboard_key_repeater_counter++;
	if ( ( KEYBOARD_PIN & KEYBOARD_MASK ) != _keyboard_status ) {
		_keyboard_key_repeater_counter = 0;
		keyboard_event_t keyboard_wanted_event = {
			.code = KEYBOARD_EVENT_NONE,
			.data = 0x00
		};
		for (uint8_t i = 0; i < 8; i++) {
			uint8_t current_key_mask = (1 << i);
			if (KEYBOARD_MASK & current_key_mask) {
				if ((KEYBOARD_PIN & current_key_mask) && !(_keyboard_status & current_key_mask)) {
					keyboard_wanted_event.code = KEYBOARD_EVENT_KEY_RELEASED;
					keyboard_wanted_event.data = current_key_mask;
					_keyboard_wanted_event = keyboard_wanted_event;
				}
				if (!(KEYBOARD_PIN & current_key_mask) && (_keyboard_status & current_key_mask)) {
					keyboard_wanted_event.code = KEYBOARD_EVENT_KEY_PRESSED;
					keyboard_wanted_event.data = current_key_mask;
					_keyboard_wanted_event = keyboard_wanted_event;
				}
			}
		}
		_keyboard_status = KEYBOARD_PIN & KEYBOARD_MASK;
	} else {
		if ( _keyboard_key_repeater_counter > KEYBOARD_KEY_REPEATER_MAX_PASS ) {
			_keyboard_key_repeater_counter = 0;
			_keyboard_wanted_event = _keyboard_previous_wanted_event;
		}
	}
}

keyboard_event_t
keyboard_get_event(void)
{
	const keyboard_event_t keyboard_wanted_event = _keyboard_wanted_event;

	if ( _keyboard_wanted_event.code != KEYBOARD_EVENT_NONE ) {
		_keyboard_previous_wanted_event = _keyboard_wanted_event;
		
		_keyboard_wanted_event.code = KEYBOARD_EVENT_NONE;
		_keyboard_wanted_event.data = 0x00;
	}

	return keyboard_wanted_event;
}
