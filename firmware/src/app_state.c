#include "app_state.h"

#include "comeos.h"

#include "lcd.h"
#include "drv_keyboard.h"
#include "windowmanager.h"


typedef enum {
		STANDBY,
		START,
		STOP
} state_t;

static state_t _current_state = STOP;

void		_app_state_display(void);

void
_app_state_init(void *data)
{
	_app_state_display();
}

void
_app_state_event_handler(const keyboard_event_t event)
{
	switch (event.code) {
		case KEYBOARD_EVENT_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_OK:
					switch (_current_state) {
						case STANDBY:
							comeos_standby();
							break;
						case START:
							comeos_start();
							break;
						case STOP:
							comeos_stop();
							break;
					}
					windowmanager_exit();
					break;
				case KEYBOARD_RIGHT:
					_current_state = (_current_state + 1)%3;
					_app_state_display();
					break;
				case KEYBOARD_LEFT:
					_current_state = (_current_state + 2)%3;
					_app_state_display();
					break;
				default:
					_app_state_display();
					break;
			}
			break;
		default:
			break;
	}
}

void
_app_state_display(void)
{
	lcd_set_mode(LCD_MODE_INVERTED);
	_app_state_display_preview();
	lcd_set_mode(LCD_MODE_NORMAL);
}

void
_app_state_display_preview(void)
{
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(0, 3);
	switch (_current_state) {
		case STANDBY:
			lcd_display_line(PSTR("Stand-by"));
			break;
		case START:
			lcd_display_line(PSTR(" Start "));
			break;
		case STOP:
			lcd_display_line(PSTR("  Stop "));
			break;
	}

	lcd_set_size(LCD_SIZE_NORMAL);
}

