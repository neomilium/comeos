#include "windowmanager.h"

#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <stdlib.h>

#include "comeos.h"

#include "lcd.h"
#include "drv_keyboard.h"
#include "drv_battery.h"
#include "drv_buzzer.h"
#include "drv_light.h"

#include "app_shots.h"
#include "app_exposition.h"

#define WM_MAX_APPLICATION_STACK_DEPTH 3
#define POWER_IDLE_DELAY 320
#define POWER_STANDBY_DELAY (POWER_IDLE_DELAY + 320)

static volatile application_t *_application_stack[WM_MAX_APPLICATION_STACK_DEPTH];
static volatile uint8_t	_current_depth = -1;

static volatile uint16_t	_inactivity_counter = 0;

static volatile uint8_t _pass_counter = 0;
static volatile uint8_t _heartbeat = 0;

typedef enum {
	POWER_STATE_ON,
	POWER_STATE_IDLE,
	POWER_STATE_STANDBY
}		power_state_t;
static volatile power_state_t _power_state;

void _windowmanager_display_statusbar(void);
void _windowmanager_display_preview_statusbar(void);

void windowmanager_resume(void);
void windowmanager_suspend(void);

ISR(INT0_vect)
{
	if( _power_state == POWER_STATE_STANDBY) {
		windowmanager_resume();
	}
}

void
windowmanager_init(void)
{
	lcd_clear();

	_windowmanager_display_preview_statusbar();
}

void 
_windowmanager_display_preview_statusbar(void)
{
	lcd_gotoxy(0, 5);
	lcd_display_string(PSTR("Progess:"));
	lcd_gotoxy(0, 6);
	lcd_display_string(PSTR("One:"));

	windowmanager_one_bar_progress();

	lcd_gotoxy(0, 7);
	lcd_display_string(PSTR("All:"));
}

void
_windowmanager_display_statusbar(void)
{
	if(_pass_counter == 0) {
		_pass_counter = 0;
		lcd_gotoxy(24, 6);
		windowmanager_one_bar_progress();

		lcd_gotoxy(24, 7);
		const uint16_t total_shots = shots_get_value();
		if ( shots_get_value() != 0 ) {
			const uint16_t current_shot = comeos_shots_count;
			if(current_shot < 100) lcd_display_char(' ');
			if(current_shot < 10) lcd_display_char(' ');
			lcd_display_number(current_shot);
	// 		lcd_set_mode(LCD_MODE_NORMAL);
			lcd_display_string(PSTR(" /"));
			if(total_shots < 100) lcd_display_char(' ');
			if(total_shots < 10) lcd_display_char(' ');
			lcd_display_number(total_shots);
		} else {
			lcd_display_string(PSTR("--- /---"));
		}

		lcd_gotoxy(72,6);
		lcd_set_size(LCD_SIZE_BIG);
		lcd_display_symbol(LCD_SYMBOL_BATTERY + battery_get_status());

		if(_heartbeat == 0) {
			lcd_display_char(' ');
			_heartbeat++;
		} else {
			switch(comeos_mode) {
				case COMEOS_MODE_ENABLED:
					lcd_display_symbol(LCD_SYMBOL_PLAY);
				break;
				case COMEOS_MODE_DISABLED:
					lcd_display_symbol(LCD_SYMBOL_STOP);
				break;
				case COMEOS_MODE_STANDBY:
					lcd_display_symbol(LCD_SYMBOL_PAUSE);
				break;
			}
			_heartbeat = 0;
		}
		lcd_set_size(LCD_SIZE_NORMAL);
	}
	_pass_counter = (_pass_counter+1)%32;
}

void
windowmanager_process_events()
{
	const keyboard_event_t event = keyboard_get_event();

	switch( _power_state ) {
		case POWER_STATE_ON:
			if((event.code == KEYBOARD_EVENT_KEY_PRESSED) || (event.code == KEYBOARD_EVENT_KEY_RELEASED)) {
				_inactivity_counter = 0;
			} else {
				_inactivity_counter++;
			}

			if((_inactivity_counter > POWER_IDLE_DELAY)) {
				light_suspend();
				_power_state = POWER_STATE_IDLE;
			} else {
				_application_stack[_current_depth]->fn_event_handler(event);
			}
			break;
		case POWER_STATE_IDLE:
			switch (event.code) {
				case KEYBOARD_EVENT_KEY_PRESSED:
					_inactivity_counter = 0;
					light_resume();
					_power_state = POWER_STATE_ON;
				break;
				default:
					if( comeos_mode != COMEOS_MODE_ENABLED) {
						_inactivity_counter++;
						if((_inactivity_counter > POWER_STANDBY_DELAY)) {
							windowmanager_suspend();
							_power_state = POWER_STATE_STANDBY;
						}
					}
				break;
			}
			break;
	}
	if( _power_state != POWER_STATE_STANDBY ) _windowmanager_display_statusbar();
}

void
windowmanager_suspend(void)
{
	lcd_clear();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	GICR |= _BV(INT0);
}

void
windowmanager_resume(void)
{
	_inactivity_counter = 0;
	_power_state = POWER_STATE_ON;
	for( uint8_t i = 0; i <= _current_depth; i++) {
		_application_stack[i]->fn_init(_application_stack[i]->user_data);
	}
	light_resume();
	_windowmanager_display_preview_statusbar();

	set_sleep_mode(SLEEP_MODE_IDLE);
	GICR &= ~(_BV(INT0));
}

void
windowmanager_launch(application_t * app)
{
	if (NULL != app) {
		++_current_depth;
		_application_stack[_current_depth] = app;
		_application_stack[_current_depth]->fn_init(_application_stack[_current_depth]->user_data);
	}
}

void
windowmanager_exit(void)
{
	if (0 != _current_depth) {
		--_current_depth;
		_application_stack[_current_depth]->fn_init(_application_stack[_current_depth]->user_data);
	}
}

void
windowmanager_one_bar_progress(void)
{
	uint16_t black_col_count;
	if( exposition_get_value() != 0 ) {
		black_col_count = ((exposition_get_value() - comeos_exposition_remaining_time) * 46) / exposition_get_value();
	} else {
		black_col_count = 0;
	}
	bf4_send_data(0x7F);
	for (uint8_t current_col = 0; current_col < 46; current_col++) {
		if( current_col < black_col_count ) {
			bf4_send_data(0x7F);
		} else {
			bf4_send_data(0x00);
		}
	}
	bf4_send_data(0x7F);
}
